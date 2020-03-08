//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#ifdef SS_PLATFORM_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#error not yet implemented
#endif

namespace ss
{

enum TcpSocketErrorCode : int
{
    SS_EAGAIN = WSAEWOULDBLOCK,
    SS_EWOULDBLOCK = WSAEWOULDBLOCK
};

}