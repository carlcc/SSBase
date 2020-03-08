//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "AsyncTcpSocket.h"
#include <uv.h>

namespace ss
{

class Loop;

class AsyncTcpSocketImpl : public AsyncTcpSocket
{
    SS_OBJECT(AsyncTcpSocketImpl, AsyncTcpSocket);

public:
    explicit AsyncTcpSocketImpl(uv_loop_t *loop);
    AsyncTcpSocketImpl(const AsyncTcpSocketImpl &) = delete;
    AsyncTcpSocketImpl(AsyncTcpSocketImpl &&) = delete;
    ~AsyncTcpSocketImpl() override;

    AsyncTcpSocketImpl &operator=(const AsyncTcpSocketImpl &) = delete;
    AsyncTcpSocketImpl &operator=(AsyncTcpSocketImpl &&) = delete;

    int Connect(const EndPoint &ep, OnConnectCb &&cb) override;
    int Connect(const String &host, uint16_t port, OnConnectCb &&cb) override;

    int Bind(const EndPoint &ep) override;

    int Listen(int backlog, OnConnectionCb &&cb) override;

    SharedPtr<AsyncTcpSocket> Accept() override;

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