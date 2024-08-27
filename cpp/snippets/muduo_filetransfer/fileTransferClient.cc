#include <unistd.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>
#include <muduo/base/Logging.h>
#include <boost/any.hpp>

using namespace muduo;
using namespace muduo::net;
const char *filepath = NULL;

using filePtr = std::shared_ptr<FILE>;

void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
{
  // LOG_INFO << "received date at " << time.toString();
  const filePtr &fp = boost::any_cast<const filePtr &>(conn->getContext());
  std::vector<char> vec(buf->peek(), buf->peek() + buf->readableBytes());
  size_t nwrite = ::fwrite(vec.data(), sizeof(char), vec.size(), fp.get());
  LOG_INFO << "BUF ADDR: " << &vec << "muduo buff: " << buf->readableBytes() << " writed: " << nwrite << "----------- need: " << vec.size() << "bytes";
  buf->retrieve(nwrite);
}

void onConnection(const TcpConnectionPtr &conn)
{
  if (conn->connected())
  {
    FILE *fp = ::fopen(filepath, "ab+");
    if (fp)
    {
      filePtr fpctx(fp, ::fclose);
      conn->setContext(fpctx);
    }
    else
    {
      LOG_INFO << "Can't create file " << filepath;
      conn->shutdown();
    }
  }
  else
  {
    LOG_INFO << "disconnected from " << conn->peerAddress().toIpPort();
    // conn->forceClose();
  }
}

int main(int argc, char *argv[])
{
  filepath = argv[1];
  EventLoop loop;
  InetAddress serverAddr(8888, "127.0.0.1");
  TcpClient client(&loop, serverAddr, "serverClient");
  client.setConnectionCallback(onConnection);
  client.setMessageCallback(onMessage);
  client.connect();
  loop.loop();

  return 0;
}