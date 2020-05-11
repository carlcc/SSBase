//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../SSBase/Object.h"
#include "../SSBase/Ptr.h"
#include "../SSBase/Str.h"
#include "SocketDef.h"
#include <cstdint>

namespace ss {

class EndPoint;

class InputStream;

class OutputStream;

class TcpSocket : public Object {
    SS_OBJECT(TcpSocket, Object);

public:
    static SharedPtr<TcpSocket> CreateSocket(IpProtocolType type);

    virtual int Connect(const EndPoint& ep) = 0;

    int Connect(const String& host, uint16_t port);

    virtual int Bind(const EndPoint& ep) = 0;

    int Bind(const String& hostAndIp);

    int Bind(const String& host, uint16_t port);

    virtual int Listen(int backlog) = 0;

    virtual SharedPtr<TcpSocket> Accept() = 0;

    virtual int Send(const void* data, uint32_t length) = 0;

    virtual int Receive(void* buf, uint32_t size) = 0;

    virtual void Close() = 0;

    virtual int ShutDown(int how) = 0;

    static int GetLastErrorCode();

    SharedPtr<InputStream> GetInputStream();

    SharedPtr<OutputStream> GetOutputStream();

    /// Get/Set Socket options

    /// How many bytes are there, so that we can invoke `Receive` to read without blocking
    virtual int Available() const = 0;

    ///
    virtual int SetNonBlocking(bool b) = 0;

    /// The send timeout if this socket uses blocking IO
    virtual int SetSendTimeout(int millis) = 0;

    /// The receive timeout if this socket uses blocking IO
    virtual int SetReceiveTimeout(int millis) = 0;

    virtual int SetNoDelay(bool b) = 0;

    virtual int SetLinger(bool b, int delaySeconds) = 0;
};

} // namespace ss