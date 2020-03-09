//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#ifdef SS_PLATFORM_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#endif

namespace ss
{

enum IpProtocolType : int {
    SS_IPV4,
    SS_IPV6
};


#ifdef SS_PLATFORM_WIN32
enum TcpSocketErrorCode : int
{
    SS_EAGAIN = WSAEWOULDBLOCK,
    SS_EWOULDBLOCK = WSAEWOULDBLOCK
};
#else
enum TcpSocketErrorCode : int
{
    SS_EAGAIN = EAGAIN,
    SS_EWOULDBLOCK = EWOULDBLOCK
};
using SOCKET = int;
const SOCKET INVALID_SOCKET = -1;
#endif


}