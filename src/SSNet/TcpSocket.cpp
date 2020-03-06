//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "TcpSocket.h"
#include "EndPoint.h"

namespace ss
{

bool TcpSocket::Bind(const String &hostAndIp)
{
    return Bind(EndPoint(hostAndIp));
}

bool TcpSocket::Bind(const String &host, uint16_t port)
{
    return Bind(EndPoint(host, port));
}

} // namespace ss
