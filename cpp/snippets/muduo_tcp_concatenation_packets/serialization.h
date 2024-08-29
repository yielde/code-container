#pragma once

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/Endian.h>
#include <muduo/net/TcpConnection.h>

#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class Serialization : noncopyable
{
public:
  using StringMessageCallback = std::function<void(const TcpConnectionPtr &, muduo::string &, Timestamp)>;
  explicit Serialization(const StringMessageCallback &cb) : messageCallback_(cb) {}

  void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime)
  {
    while (buf->readableBytes() >= headerLen)
    {
      // 解包消息长度
      const void *data = buf->peek();
      int32_t bigendian32 = *static_cast<const int32_t *>(data);
      const int32_t len = muduo::net::sockets::networkToHost32(bigendian32); // 消息体长度
      if (len > 65536 || len < 0)                                            // 不超过64k消息
      {
        LOG_ERROR << "invalid length";
        conn->shutdown();
        break;
      }
      else if (buf->readableBytes() >= len + headerLen)
      {
        buf->retrieve(headerLen);
        muduo::string message(buf->peek(), len); // 拿消息
        messageCallback_(conn, message, receiveTime);
        buf->retrieve(len);
      }
      else
      {
        // 消息不完整，下次再读
        break;
      }
    }
  }

  void send(const TcpConnectionPtr &conn, const StringPiece &message)
  {
    Buffer buf;
    buf.append(message.begin(), message.size());
    int32_t len = static_cast<int32_t>(message.size());
    int32_t bigendian32 = muduo::net::sockets::hostToNetwork32(len);
    // 填充消息的长度（填充到muduo预留的prependable bytes）
    // |prependable bytes|messagebody| -> |bigendian32|messagebody|
    buf.prepend(&bigendian32, sizeof(bigendian32));
    conn->send(&buf);
  }

private:
  StringMessageCallback messageCallback_;
  const static size_t headerLen = sizeof(int32_t); // 消息长度 |headerLen|messagebody|
};