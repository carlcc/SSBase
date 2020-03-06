//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Loop.h"
#include "TcpSocket.h"
#include "TcpSocketImpl.h"
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

void *Loop::GetHandle()
{
    return &impl_->loop_;
}

SharedPtr<TcpSocket> Loop::CreateTcpSocket()
{
    return MakeShared<TcpSocketImpl>(this);
}

} // namespace ss