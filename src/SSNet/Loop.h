//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Object.h>
#include <SSBase/Ptr.h>

namespace ss
{

class AsyncTcpSocket;

class Loop : public Object
{
    SS_OBJECT(Loop, Object);

public:
    Loop();
    Loop(const Loop &l) = delete;
    Loop(Loop &&l) = delete;
    ~Loop() override;

    Loop &operator=(const Loop &l) = delete;
    Loop &operator=(Loop &&l) = delete;

    int Run();

    void Stop();

    ////
    SharedPtr<AsyncTcpSocket> CreateTcpSocket();

private:
    class LoopImpl;
    LoopImpl *impl_;
};

} // namespace ss
