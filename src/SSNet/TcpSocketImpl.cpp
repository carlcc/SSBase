//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "TcpSocketImpl.h"
#include "../SSBase/ObjectPool.h"
#include "EndPoint.h"
#include "EndPointInternal.h"
#include "Loop.h"

namespace ss
{

struct TcpSocketImpl::Data
{
    uv_tcp_t handle_{};
    OnDataCb onDataCb_{nullptr};
    OnCloseCb onCloseCb_{nullptr};
    OnConnectionCb onConnectionCb{nullptr};
    TcpSocketImpl *self_;
};

TcpSocketImpl::TcpSocketImpl(uv_loop_t *loop) : data_(nullptr), loop_(nullptr)
{
    if (loop == nullptr)
    {
        return;
    }

    data_ = new Data;
    if (0 != uv_tcp_init(loop, (uv_tcp_t *)data_))
    {
        delete data_;
        data_ = nullptr;
    }
    else
    {
        data_->handle_.data = data_;
        data_->self_ = this;
        loop_ = loop;
    }
}

TcpSocketImpl::~TcpSocketImpl()
{
    TcpSocketImpl::Close(nullptr);
}

int TcpSocketImpl::Connect(const EndPoint &ep, OnConnectCb &&cb)
{
    if (data_ == nullptr)
    {
        return UV_EINVAL;
    }

    struct Req
    {
        uv_connect_t req{};
        OnConnectCb cb{nullptr};
    };
    auto *req = new Req;
    req->cb = std::forward<OnConnectCb>(cb);
    return uv_tcp_connect((uv_connect_t *)req, (uv_tcp_t *)data_, &ep.impl_->addr_, [](uv_connect_t *req, int status) {
        Req *r = (Req *)req;
        if (r->cb != nullptr)
        {
            r->cb(status);
        }
        delete r;
    });
}

int TcpSocketImpl::Connect(const String &host, uint16_t port, OnConnectCb &&cb)
{
    return Connect(EndPoint(host, port), std::forward<OnConnectCb>(cb));
}

int TcpSocketImpl::Bind(const EndPoint &ep)
{
    if (data_ == nullptr || !ep.IsValid())
    {
        return UV_EINVAL;
    }

    return uv_tcp_bind((uv_tcp_t *)data_, &ep.impl_->addr_, 0);
}

int TcpSocketImpl::Listen(int backlog, TcpSocket::OnConnectionCb &&cb)
{
    if (data_ == nullptr)
    {
        return UV_EINVAL;
    }
    data_->onConnectionCb = std::forward<OnConnectionCb>(cb);
    return uv_listen((uv_stream_t *)data_, backlog, [](uv_stream_t *server, int status) {
        auto *data = (Data *)server->data;
        if (data->onConnectionCb != nullptr)
        {
            data->onConnectionCb(data->self_, status);
        }
    });
}

SharedPtr<TcpSocket> TcpSocketImpl::Accept()
{
    auto client = MakeShared<TcpSocketImpl>(loop_);
    SSASSERT(client->data_ != nullptr);

    if (0 != uv_accept((uv_stream_t *)data_, (uv_stream_t *)&client->data_->handle_))
    {
        return nullptr;
    }
    return client;
}

int TcpSocketImpl::Send(const void *data, uint32_t length, OnSendCb &&cb)
{
    if (data_ == nullptr)
    {
        return UV_EINVAL;
    }

    struct Req
    {
        uv_write_t req{};
        OnSendCb cb{nullptr};
    };
    Req *req = new Req;
    req->cb = std::forward<OnSendCb>(cb);

    uv_buf_t buf;
    buf.base = (char *)data;
    buf.len = length;
    return uv_write((uv_write_t *)req, (uv_stream_t *)data_, &buf, 1, [](uv_write_t *req, int status) {
        auto *r = (Req *)req;
        if (r->cb != nullptr)
        {
            r->cb(status);
        }
        delete r;
    });
}

static void UvAllocCb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = new char[4096];
    buf->len = 4096;
}

int TcpSocketImpl::StartReceive(OnDataCb &&cb)
{
    if (data_ == nullptr)
    {
        return UV_EINVAL;
    }
    data_->onDataCb_ = std::forward<OnDataCb>(cb);

    // TODO: Optimize memory
    return uv_read_start((uv_stream_s *)data_, UvAllocCb, [](uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
        auto *data = (Data *)stream->data;
        data->onDataCb_(nread, buf->base);
        delete buf->base;
    });
}

void TcpSocketImpl::StopReceive()
{
    if (data_ == nullptr)
    {
        return;
    }
    uv_read_stop((uv_stream_s *)data_);
}

void TcpSocketImpl::Close(OnCloseCb &&cb)
{
    if (data_ == nullptr)
    {
        return;
    }

    data_->onCloseCb_ = std::forward<OnCloseCb>(cb);
    uv_close((uv_handle_t *)data_, [](uv_handle_t *handle) {
        auto *data = (Data *)handle;
        if (data->onCloseCb_ != nullptr)
        {
            data->onCloseCb_();
        }
        delete data;
    });
    data_ = nullptr;
}

int TcpSocketImpl::ShutDown(TcpSocket::OnShutDownCb &&cb)
{
    if (data_ == nullptr)
    {
        return UV_EINVAL;
    }

    struct Req
    {
        uv_shutdown_t req{};
        OnShutDownCb cb{nullptr};
    };
    Req *req = new Req;
    req->cb = std::forward<OnShutDownCb>(cb);

    return uv_shutdown((uv_shutdown_t *)req, (uv_stream_t *)data_, [](uv_shutdown_t *req, int status) {
        auto *r = (Req *)req;
        if (r->cb != nullptr)
        {
            r->cb(status);
        }
        delete r;
    });
}

} // namespace ss