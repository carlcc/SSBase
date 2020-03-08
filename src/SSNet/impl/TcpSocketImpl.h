//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../../SSNet/TcpSocket.h"
#include "../EndPoint.h"
#include "../EndPointInternal.h"

#ifdef SS_PLATFORM_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#error not yet implemented
#endif

namespace ss
{

#ifdef SS_PLATFORM_WIN32
// Initialize Winsock.

class __WindowsSocketInitHelper
{
public:
    __WindowsSocketInitHelper()
    {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR)
        {
            wprintf(L"WSAStartup failed with error: %ld\n", iResult);
            exit(1);
        }
    }
    ~__WindowsSocketInitHelper()
    {
        WSACleanup();
    }
};
static __WindowsSocketInitHelper __windowsSocketInitHelper;
#endif

class TcpSocketImpl : public TcpSocket
{
    SS_OBJECT(TcpSocketImpl, TcpSocket)
public:
    // This object will take the ownship of fd
    TcpSocketImpl(SOCKET fd) : sockFd_(fd)
    {
    }
    ~TcpSocketImpl()
    {
        Close();
    }

    int Connect(const EndPoint &ep) override
    {
        int nameLen = ep.IsIPV4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
        return connect(sockFd_, &ep.impl_->addr_, nameLen);
    }

    int Bind(const EndPoint &ep) override
    {
        int nameLen = ep.IsIPV4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
        return bind(sockFd_, &ep.impl_->addr_, nameLen);
    }

    int Listen(int backlog) override
    {
        return listen(sockFd_, backlog);
    }

    SharedPtr<TcpSocket> Accept() override
    {
        auto client = accept(sockFd_, nullptr, nullptr);
        if (INVALID_SOCKET == client)
        {
            return nullptr;
        }
        return MakeShared<TcpSocketImpl>(client);
    }

    int Send(const void *data, uint32_t length) override
    {
        return send(sockFd_, (const char *)data, int(length), 0);
    }

    int Receive(void *buf, uint32_t size) override
    {
        return recv(sockFd_, (char *)buf, int(size), 0);
    }

    void Close() override
    {
        if (INVALID_SOCKET != sockFd_)
        {
            closesocket(sockFd_);
            sockFd_ = INVALID_SOCKET;
        }
    }

    int ShutDown(int how) override
    {
        return shutdown(sockFd_, how);
    }

    int Available() const override
    {
        u_long avalable = 0;
        if (0 == ioctlsocket(sockFd_, FIONREAD, &avalable))
        {
            return (int)avalable;
        }
        return -1;
    }

    int SetNonBlocking(bool b) override
    {
        u_long mode = b ? 1 : 0;
        return ioctlsocket(sockFd_, FIONBIO, &mode);
    }

    int SetSendTimeout(int millis) override
    {
        DWORD timeout = millis < 0 ? 0 : millis;
        return setsockopt(sockFd_, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
    }

    int SetReceiveTimeout(int millis) override
    {
        DWORD timeout = millis < 0 ? 0 : millis;
        return setsockopt(sockFd_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    }

private:
    SOCKET sockFd_;
};
} // namespace ss