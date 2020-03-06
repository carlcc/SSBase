//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../SSBase/Object.h"
#include "../SSBase/Ptr.h"
#include <functional>

namespace ss
{

class EndPoint;

class TcpSocket : public Object
{
    SS_OBJECT(TcpSocket, Object);

public:
    using OnConnectCb = std::function<void(int status)>; // status will be 0 in case of success, < 0 otherwise.
    using OnCloseCb = std::function<void()>;
    using OnSendCb = std::function<void(int status)>; // status will be 0 in case of success, < 0 otherwise.
    // nread is > 0 if there is data available or < 0 on error. When we’ve reached EOF, nread will be set to UV_EOF.
    // When nread < 0, the buf parameter might not point to a valid buffer;
    using OnDataCb = std::function<void(ssize_t nread, const char *buf)>;
    // status will be 0 in case of success, else < 0.
    using OnConnectionCb = std::function<void(TcpSocket *server, int status)>;
    using OnShutDownCb = std::function<void(int status)>;

    static const String kSigClose;

    virtual bool Connect(const EndPoint &ep, OnConnectCb &&cb) = 0;

    virtual bool Connect(const String &host, uint16_t port, OnConnectCb &&cb) = 0;

    virtual bool Bind(const EndPoint &ep) = 0;

    bool Bind(const String &hostAndIp);

    bool Bind(const String &host, uint16_t port) ;

    virtual bool Listen(int backlog, OnConnectionCb &&cb) = 0;

    virtual SharedPtr<TcpSocket> Accept() = 0;

    // The data pointed by `data` should keey valid untile `cb` is called.
    virtual bool Send(const void *data, uint32_t length, OnSendCb &&cb) = 0;

    virtual bool StartReceive(OnDataCb &&cb) = 0;

    virtual void StopReceive() = 0;

    virtual void Close(OnCloseCb &&cb) = 0;

    virtual bool ShutDown(OnShutDownCb &&cb) = 0;
};

} // namespace ss
