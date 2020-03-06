//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "TcpSocket.h"
#include "EndPoint.h"

namespace ss
{

int TcpSocket::Bind(const String &hostAndIp)
{
    return Bind(EndPoint(hostAndIp));
}

int TcpSocket::Bind(const String &host, uint16_t port)
{
    return Bind(EndPoint(host, port));
}

} // namespace ss
