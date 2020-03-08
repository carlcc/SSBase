//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "TcpSocket.h"
#include "../SSIO/stream/InputStream.h"
#include "../SSIO/stream/OutputStream.h"
#include "EndPoint.h"
#include "impl/TcpSocketImpl.h"
#include <SSIO/stream/StreamConstant.h>

namespace ss
{

int TcpSocket::Connect(const String &host, uint16_t port)
{
    return Connect(EndPoint(host, port));
}

int TcpSocket::Bind(const String &hostAndIp)
{
    return Bind(EndPoint(hostAndIp));
}

int TcpSocket::Bind(const String &host, uint16_t port)
{
    return Bind(EndPoint(host, port));
}

SharedPtr<TcpSocket> TcpSocket::CreateSocket()
{
#ifdef SS_PLATFORM_WIN32
    // Create Socket and check if error occured afterwards
    SOCKET fd = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == fd)
    {
        return nullptr;
    }
#else
#error NYI
#endif
    return MakeShared<TcpSocketImpl>(fd);
}

class TcpSocketInputStream : public InputStream
{
    SS_OBJECT(TcpSocketInputStream, InputStream);

public:
    explicit TcpSocketInputStream(TcpSocket *socket) : socket_(socket)
    {
    }

    int32_t Read(void *buf, uint32_t count) override
    {
        SSASSERT(socket_ != nullptr);
        auto ret = socket_->Receive(buf, count);
        if (ret == 0)
        {
            return StreamConstant::kEof; // Socket return 0 on EOF
        }
        if (ret < 0)
        {
            auto code = TcpSocket::GetLastErrorCode();
            if (code == SS_EAGAIN || code == SS_EWOULDBLOCK)
            {
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
        if (b == 1)
        {
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

class TcpSocketOutputStream : public OutputStream
{
    SS_OBJECT(TcpSocketOutputStream, OutputStream)

public:
    explicit TcpSocketOutputStream(TcpSocket *socket) : socket_(socket)
    {
    }

    int32_t Write(const void *data, uint32_t count) override
    {
        SSASSERT(socket_ != nullptr);
        auto ret = socket_->Send(data, count);
        if (ret >= 0)
        {
            return ret;
        }
        return StreamConstant::kUnknown;
    }

    int Write(uint8_t byte) override
    {
        auto ret = Write(&byte, 1);
        if (ret == 1)
        {
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
    new TcpSocketOutputStream(this);
    return MakeShared<TcpSocketOutputStream>(this);
}

int TcpSocket::GetLastErrorCode()
{
#ifdef SS_PLATFORM_WIN32
    return WSAGetLastError();
#else
#error NYI, return errno?
#endif
}

} // namespace ss