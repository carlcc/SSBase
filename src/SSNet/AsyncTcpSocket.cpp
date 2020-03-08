//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "AsyncTcpSocket.h"
#include "EndPoint.h"

namespace ss
{

int AsyncTcpSocket::Connect(const String &host, uint16_t port, OnConnectCb &&cb)
{
    return Connect(EndPoint(host, port), std::forward<OnConnectCb>(cb));
}

int AsyncTcpSocket::Bind(const String &hostAndIp)
{
    return Bind(EndPoint(hostAndIp));
}

int AsyncTcpSocket::Bind(const String &host, uint16_t port)
{
    return Bind(EndPoint(host, port));
}

} // namespace ss
