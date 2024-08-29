#include "serialization.h"
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoopThread.h>
#include <iostream>
#include <mutex>

class Client : noncopyable
{
public:
  Client(EventLoop *loop, const InetAddress &serverAddr, const std::string &clientName)
      : client_(loop, serverAddr, clientName),
        serial_(std::bind(&Client::onStringMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
  {
    client_.setMessageCallback(std::bind(&Serialization::onMessage, &serial_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    client_.setConnectionCallback(std::bind(&Client::onConnection, this, std::placeholders::_1));
    client_.enableRetry();
  }

  void connect()
  {
    client_.connect();
  }

  void disconnect()
  {
    client_.disconnect();
  }

  // 主线程调用
  void sendMsg(const StringPiece &message)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (connection_)
    {
      serial_.send(connection_, message);
    }
  }

private:
  using StringMessageCallback = std::function<void(const TcpConnectionPtr &, muduo::string &, Timestamp)>;

  void onStringMessage(const TcpConnectionPtr &conn, muduo::string &message, Timestamp receiveTime)
  {
    std::cout << "<<< " << message << std::endl;
  }

  void onConnection(const TcpConnectionPtr &conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " ---> " << conn->peerAddress().toIpPort() << " is " << (conn->connected() ? "UP" : "DOWN");
    std::lock_guard<std::mutex> lock(mutex_);
    if (conn->connected())
    {
      connection_ = conn;
    }
    else
    {
      connection_.reset();
    }
  }

private:
  TcpClient client_;
  Serialization serial_;
  std::mutex mutex_;
  TcpConnectionPtr connection_; // TcpConnection 线程
};

int main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    std::cout << "need ip port" << std::endl;
    return -1;
  }

  EventLoopThread loopthread;
  InetAddress serverAddr(argv[1], atoi(argv[2]));
  Client client(loopthread.startLoop(), serverAddr, "chatClient"); // 子线程阻塞在loop.loop() (epoll_wait)
  client.connect();
  // 主线程处理终端IO，子线程处理网络IO
  std::string line;
  while (std::getline(std::cin, line))
  {
    client.sendMsg(line);
  }
  client.disconnect();

  return 0;
}