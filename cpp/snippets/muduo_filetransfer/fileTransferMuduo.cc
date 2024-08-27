#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <boost/any.hpp>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

const int bufferSize = 64 * 1024;
const char *filename = NULL;
using FilePtr = std::shared_ptr<FILE>;

void onConnection(const TcpConnectionPtr &conn)
{
  LOG_INFO << "fileserver - " << conn->localAddress().toIpPort() << " -> " << conn->peerAddress().toIpPort() << "is " << (conn->connected() ? "UP" : "DOWN");
  if (conn->connected())
  {
    LOG_INFO << "Send file " << filename;
    FILE *fp = ::fopen(filename, "rb");
    if (fp)
    {
      FilePtr fpctx(fp, ::fclose);
      conn->setContext(fpctx);
      char buf[bufferSize];
      size_t nread = ::fread(buf, 1, sizeof(buf), fp);
      LOG_INFO << "buf size: " << sizeof(buf) << "nread: " << nread;
      conn->send(buf, nread);
    }
    else
    {
      LOG_WARN << "No Such file " << filename;
      conn->shutdown();
    }
  }
  else
  {
    LOG_INFO << "fileserver - close";
  }
}

void onWriteComplete(const TcpConnectionPtr &conn)
{
  const FilePtr &fp = boost::any_cast<const FilePtr &>(conn->getContext());
  char buf[bufferSize];
  size_t nread = ::fread(buf, 1, sizeof(buf), fp.get());
  if (nread > 0)
  {
    conn->send(buf, nread);
    LOG_INFO << "buf size: " << sizeof(buf) << "nread: " << nread;
  }
  else
  {
    conn->shutdown();
    LOG_INFO << "file transfer complete";
  }
}

int main(int argc, char *argv[])
{
  filename = argv[1];

  EventLoop loop;
  InetAddress listenAddr(8888, "0.0.0.0");
  TcpServer server(&loop, listenAddr, "fileserver");
  server.setConnectionCallback(onConnection);
  server.setWriteCompleteCallback(onWriteComplete);
  server.start();
  loop.loop();

  return 0;
}