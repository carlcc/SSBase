//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "TcpSocket.h"
#include "../SSIO/stream/InputStream.h"
#include "../SSIO/stream/OutputStream.h"
#include "EndPoint.h"
#include "SocketDef.h"
#include "impl/TcpSocketImpl.h"
#include <SSIO/stream/StreamConstant.h>

namespace ss {

int TcpSocket::Connect(const String& host, uint16_t port)
{
    return Connect(EndPoint(host, port));
}

int TcpSocket::Bind(const String& hostAndIp)
{
    return Bind(EndPoint(hostAndIp));
}

int TcpSocket::Bind(const String& host, uint16_t port)
{
    return Bind(EndPoint(host, port));
}

SharedPtr<TcpSocket> TcpSocket::CreateSocket(IpProtocolType type)
{
    // Create Socket and check if error occured afterwards
    int domain = type == IpProtocolType::SS_IPV4 ? AF_INET : AF_INET6;

    // NOTE: On windows, the domain may be AF_UNSPEC,
    // then this socket can then bind to both IPv6 address and IPv4 Address
    // But it seems linux does not support this feature.
    SOCKET fd = socket(domain, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == fd) {
        return nullptr;
    }
    if (type == IpProtocolType::SS_IPV6) {
        // Disable V6ONLY option for IPv6, ignore the return code
#ifdef SS_PLATFORM_WIN32
#error NYI
#else
        int v6only = 0;
        setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only));
#endif
    }
    return MakeShared<TcpSocketImpl>(fd);
}

class TcpSocketInputStream : public InputStream {
    SS_OBJECT(TcpSocketInputStream, InputStream);

public:
    explicit TcpSocketInputStream(TcpSocket* socket)
        : socket_(socket)
    {
    }

    int32_t Read(void* buf, uint32_t count) override
    {
        SSASSERT(socket_ != nullptr);
        auto ret = socket_->Receive(buf, count);
        if (ret == 0) {
            return StreamConstant::kEof; // Socket return 0 on EOF
        }
        if (ret < 0) {
            auto code = TcpSocket::GetLastErrorCode();
            if (code == SS_EAGAIN || code == SS_EWOULDBLOCK) {
                return 0;
            }
            return StreamConstant::kUnknown;
        }
        return ret;
    }

    int Read() override
    {
        uint8_t b;
        int ret = Read(&b, 1);
        if (b == 1) {
            return b;
        }
        return ret;
    }

    int32_t Available() const override
    {
        SSASSERT(socket_ != nullptr);
        return socket_->Available();
    }

    void Close() override
    {
        socket_ = nullptr;
    }

    bool IsValid() const override
    {
        return socket_ != nullptr;
    }

private:
    SharedPtr<TcpSocket> socket_;
};

SharedPtr<InputStream> TcpSocket::GetInputStream()
{
    return MakeShared<TcpSocketInputStream>(this);
}

class TcpSocketOutputStream : public OutputStream {
    SS_OBJECT(TcpSocketOutputStream, OutputStream)

public:
    explicit TcpSocketOutputStream(TcpSocket* socket)
        : socket_(socket)
    {
    }

    int32_t Write(const void* data, uint32_t count) override
    {
        SSASSERT(socket_ != nullptr);
        auto ret = socket_->Send(data, count);
        if (ret >= 0) {
            return ret;
        }
        return StreamConstant::kUnknown;
    }

    int Write(uint8_t byte) override
    {
        auto ret = Write(&byte, 1);
        if (ret == 1) {
            return byte;
        }
        return ret;
    }
    void Close() override
    {
        socket_ = nullptr;
    }
    bool IsValid() const override
    {
        return socket_ != nullptr;
    }
    int32_t Flush() override
    {
        return StreamConstant::kOk;
    }

private:
    SharedPtr<TcpSocket> socket_;
};

SharedPtr<OutputStream> TcpSocket::GetOutputStream()
{
    return MakeShared<TcpSocketOutputStream>(this);
}

int TcpSocket::GetLastErrorCode()
{
#ifdef SS_PLATFORM_WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

} // namespace ss