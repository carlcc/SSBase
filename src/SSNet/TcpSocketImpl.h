//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "TcpSocket.h"
#include <uv.h>

namespace ss
{

class Loop;

class TcpSocketImpl : public TcpSocket
{
    SS_OBJECT(TcpSocketImpl, TcpSocket);

public:
    explicit TcpSocketImpl(Loop *loop);
    TcpSocketImpl(const TcpSocketImpl &) = delete;
    TcpSocketImpl(TcpSocketImpl &&) = delete;
    ~TcpSocketImpl() override;

    TcpSocketImpl &operator=(const TcpSocketImpl &) = delete;
    TcpSocketImpl &operator=(TcpSocketImpl &&) = delete;

    bool Connect(const EndPoint &ep, OnConnectCb &&cb) override;
    bool Connect(const String &host, uint16_t port, OnConnectCb &&cb) override;

    bool Bind(const EndPoint &ep) override;

    bool Listen(int backlog, OnConnectionCb &&cb) override;

    SharedPtr<TcpSocket> Accept() override;

    bool Send(const void *data, uint32_t length, OnSendCb &&cb) override;

    bool StartReceive(OnDataCb &&cb) override;

    void StopReceive() override;

    void Close(OnCloseCb &&cb) override;

    bool ShutDown(OnShutDownCb &&cb) override;

private:
    struct Data;
    Data *data_;
    Loop *loop_;
};

} // namespace ss