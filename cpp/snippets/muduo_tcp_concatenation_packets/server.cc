#include "serialization.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <set>
#include <mutex>
class Server : noncopyable
{
public:
  Server(EventLoop *loop, InetAddress &serverAddr, const std::string &serverName)
      : server_(loop, serverAddr, serverName),
        serial_(std::bind(&Server::onStringMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
  {
    server_.setConnectionCallback(std::bind(&Server::onConnection, this, std::placeholders::_1));
    server_.setMessageCallback(std::bind(&Serialization::onMessage, &serial_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  }

  void setThreadNum(int nums)
  {
    server_.setThreadNum(nums);
  }

  void start()
  {
    server_.start();
  }

private:
  void onStringMessage(const TcpConnectionPtr &conn, const StringPiece &message, Timestamp receiveTime)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    for (std::set<TcpConnectionPtr>::iterator it = connections_.begin(); it != connections_.end(); ++it)
    {
      serial_.send(*it, message);
    }
  }

  void onConnection(const TcpConnectionPtr &conn)
  {
    LOG_INFO << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort();
    std::lock_guard<std::mutex> lock(mutex_);
    if (conn->connected())
    {
      connections_.insert(conn);
    }
    else
    {
      connections_.erase(conn);
    }
  }

  TcpServer server_;
  std::set<TcpConnectionPtr> connections_;
  std::mutex mutex_;
  Serialization serial_;
};

int main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    LOG_INFO << "use ip port";
    return -1;
  }

  EventLoop loop;
  InetAddress serverAddr(atoi(argv[2]), argv[1]);
  Server server(&loop, serverAddr, "ChatServer");
  server.setThreadNum(2);
  server.start();
  loop.loop();
  return 0;
}