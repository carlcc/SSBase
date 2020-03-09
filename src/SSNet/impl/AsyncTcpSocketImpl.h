//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../../SSNet/AsyncTcpSocket.h"
#include <uv.h>

namespace ss
{

class Loop;

class AsyncTcpSocketImpl : public AsyncTcpSocket
{
    SS_OBJECT(AsyncTcpSocketImpl, AsyncTcpSocket);

public:
    struct Data;

    // This object will take the ownship of handle
    explicit AsyncTcpSocketImpl(Data *handle);
    AsyncTcpSocketImpl(const AsyncTcpSocketImpl &) = delete;
    AsyncTcpSocketImpl(AsyncTcpSocketImpl &&) = delete;
    ~AsyncTcpSocketImpl() override;

    AsyncTcpSocketImpl &operator=(const AsyncTcpSocketImpl &) = delete;
    AsyncTcpSocketImpl &operator=(AsyncTcpSocketImpl &&) = delete;

    int Connect(const EndPoint &ep, OnConnectCb &&cb) override;

    int Bind(const EndPoint &ep) override;

    int Listen(int backlog, OnConnectionCb &&cb) override;

    SharedPtr<AsyncTcpSocket> Accept() override;

    int Send(const void *data, uint32_t length, OnSendCb &&cb) override;

    int StartReceive(OnDataCb &&cb) override;

    void StopReceive() override;

    void Close(OnCloseCb &&cb) override;

    int ShutDown(OnShutDownCb &&cb) override;

private:
    Data *data_;
};

struct AsyncTcpSocketImpl::Data
{
    uv_tcp_t handle_{};
    OnDataCb onDataCb_{nullptr};
    OnCloseCb onCloseCb_{nullptr};
    OnConnectionCb onConnectionCb{nullptr};
    AsyncTcpSocketImpl *self_;
};

} // namespace ss