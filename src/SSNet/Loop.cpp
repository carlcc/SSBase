//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Loop.h"
#include "AsyncTcpSocket.h"
#include "impl/AsyncTcpSocketImpl.h"
#include <uv.h>

namespace ss
{

class Loop::LoopImpl
{
public:
    LoopImpl() : loop_{}
    {
        uv_loop_init(&loop_);
    }

    ~LoopImpl()
    {
        uv_loop_close(&loop_);
    }

    int Run()
    {
        return uv_run(&loop_, UV_RUN_DEFAULT);
    }

    void Stop()
    {
        uv_stop(&loop_);
    }

    uv_loop_t loop_;
};

Loop::Loop() : impl_(new LoopImpl)
{
}

Loop::~Loop()
{
    delete impl_;
}

int Loop::Run()
{
    return impl_->Run();
}

void Loop::Stop()
{
    impl_->Stop();
}

SharedPtr<AsyncTcpSocket> Loop::CreateTcpSocket()
{
    auto *data = new AsyncTcpSocketImpl::Data;

    if (0 != uv_tcp_init(&impl_->loop_, (uv_tcp_t *)data))
    {
        delete data;
        return nullptr;
    }
    auto s = MakeShared<AsyncTcpSocketImpl>(data);
    data->handle_.data = data;
    data->self_ = s.Get();
    return s;
}

} // namespace ss