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
    explicit TcpSocketImpl(uv_loop_t *loop);
    TcpSocketImpl(const TcpSocketImpl &) = delete;
    TcpSocketImpl(TcpSocketImpl &&) = delete;
    ~TcpSocketImpl() override;

    TcpSocketImpl &operator=(const TcpSocketImpl &) = delete;
    TcpSocketImpl &operator=(TcpSocketImpl &&) = delete;

    int Connect(const EndPoint &ep, OnConnectCb &&cb) override;
    int Connect(const String &host, uint16_t port, OnConnectCb &&cb) override;

    int Bind(const EndPoint &ep) override;

    int Listen(int backlog, OnConnectionCb &&cb) override;

    SharedPtr<TcpSocket> Accept() override;

    int Send(const void *data, uint32_t length, OnSendCb &&cb) override;

    int StartReceive(OnDataCb &&cb) override;

    void StopReceive() override;

    void Close(OnCloseCb &&cb) override;

    int ShutDown(OnShutDownCb &&cb) override;

private:
    struct Data;
    Data *data_;
    uv_loop_t *loop_;
};

} // namespace ss