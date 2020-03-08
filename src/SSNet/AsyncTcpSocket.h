//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../SSBase/Object.h"
#include "../SSBase/Ptr.h"
#include <functional>
#include <uv.h>

namespace ss
{

class EndPoint;

class AsyncTcpSocket : public Object
{
    SS_OBJECT(AsyncTcpSocket, Object);

public:
    using OnConnectCb = std::function<void(int status)>; // status will be 0 in case of success, < 0 otherwise.
    using OnCloseCb = std::function<void()>;
    using OnSendCb = std::function<void(int status)>; // status will be 0 in case of success, < 0 otherwise.
    // nread is > 0 if there is data available or < 0 on error. When we’ve reached EOF, nread will be set to UV_EOF.
    // When nread < 0, the buf parameter might not point to a valid buffer;
    using OnDataCb = std::function<void(ssize_t nread, const char *buf)>;
    // status will be 0 in case of success, else < 0.
    using OnConnectionCb = std::function<void(AsyncTcpSocket *server, int status)>;
    using OnShutDownCb = std::function<void(int status)>;

    static const String kSigClose;

    virtual int Connect(const EndPoint &ep, OnConnectCb &&cb) = 0;

    int Connect(const String &host, uint16_t port, OnConnectCb &&cb);

    virtual int Bind(const EndPoint &ep) = 0;

    int Bind(const String &hostAndIp);

    int Bind(const String &host, uint16_t port);

    virtual int Listen(int backlog, OnConnectionCb &&cb) = 0;

    virtual SharedPtr<AsyncTcpSocket> Accept() = 0;

    // The data pointed by `data` should keey valid untile `cb` is called.
    virtual int Send(const void *data, uint32_t length, OnSendCb &&cb) = 0;

    virtual int StartReceive(OnDataCb &&cb) = 0;

    virtual void StopReceive() = 0;

    virtual void Close(OnCloseCb &&cb) = 0;

    virtual int ShutDown(OnShutDownCb &&cb) = 0;

};

} // namespace ss
