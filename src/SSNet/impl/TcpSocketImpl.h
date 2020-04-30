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
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace ss {

#ifdef SS_PLATFORM_WIN32
// Initialize Winsock.

class __WindowsSocketInitHelper {
public:
    __WindowsSocketInitHelper()
    {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR) {
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

class TcpSocketImpl : public TcpSocket {
    SS_OBJECT(TcpSocketImpl, TcpSocket)
public:
    // This object will take the ownship of fd
    TcpSocketImpl(SOCKET fd)
        : sockFd_(fd)
    {
    }
    ~TcpSocketImpl()
    {
        Close();
    }

    int Connect(const EndPoint& ep) override
    {
        int nameLen = ep.IsIPV4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
        return connect(sockFd_, &ep.impl_->addr_, nameLen);
    }

    int Bind(const EndPoint& ep) override
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
        if (INVALID_SOCKET == client) {
            return nullptr;
        }
        return MakeShared<TcpSocketImpl>(client);
    }

    int Send(const void* data, uint32_t length) override
    {
        return send(sockFd_, (const char*)data, int(length), 0);
    }

    int Receive(void* buf, uint32_t size) override
    {
        return recv(sockFd_, (char*)buf, int(size), 0);
    }

    void Close() override
    {
        if (INVALID_SOCKET != sockFd_) {
#ifdef SS_PLATFORM_WIN32
            closesocket(sockFd_);
#else
            close(sockFd_);
#endif
            sockFd_ = INVALID_SOCKET;
        }
    }

    int ShutDown(int how) override
    {
        return shutdown(sockFd_, how);
    }

    int Available() const override
    {

#ifdef SS_PLATFORM_WIN32
        u_long avalable = 0;
        if (0 == ioctlsocket(sockFd_, FIONREAD, &avalable))
#else
        u_long avalable = 0;
        if (0 == ioctl(sockFd_, FIONREAD, (char*)&avalable))
#endif
        {
            return (int)avalable;
        }
        return -1;
    }

    int SetNonBlocking(bool b) override
    {
        u_long mode = b ? 1 : 0;
#ifdef SS_PLATFORM_WIN32
        return ioctlsocket(sockFd_, FIONBIO, &mode);
#else
        return ioctl(sockFd_, FIONBIO, (char*)&mode);
#endif
    }

    int SetSendTimeout(int millis) override
    {
#ifdef SS_PLATFORM_WIN32
        DWORD timeout = millis < 0 ? 0 : millis;
#else
        if (millis < 0) {
            millis = 0;
        }
        struct timeval timeout = { millis / 1000, millis % 1000 };
#endif
        return setsockopt(sockFd_, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    }

    int SetReceiveTimeout(int millis) override
    {
#ifdef SS_PLATFORM_WIN32
        DWORD timeout = millis < 0 ? 0 : millis;
#else
        if (millis < 0) {
            millis = 0;
        }
        struct timeval timeout = { millis / 1000, millis % 1000 };
#endif
        return setsockopt(sockFd_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    }

    int SetNoDelay(bool b) override
    {
        int flag = b ? 1 : 0;
        return setsockopt(sockFd_, IPPROTO_TCP, TCP_NODELAY, (const char*)&flag, sizeof(flag));
    }

    int SetLinger(bool b, int delaySeconds) override
    {
        linger l {};
        l.l_onoff = b ? 1 : 0;
        l.l_onoff = delaySeconds;

#ifdef SS_PLATFORM_WIN32
        return setsockopt(sockFd_, SOL_SOCKET, SO_LINGER, (const char*)&l, sizeof(linger));
#else
        return setsockopt(sockFd_, SOL_SOCKET, SO_LINGER, &l, sizeof(linger));
#endif
    }

private:
    SOCKET sockFd_;
};
} // namespace ss