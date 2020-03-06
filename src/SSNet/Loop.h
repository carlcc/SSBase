//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Object.h>
#include <SSBase/Ptr.h>

namespace ss
{

class TcpSocket;

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

    void *GetHandle();

    ////
    SharedPtr<TcpSocket> CreateTcpSocket();

private:
    class LoopImpl;
    LoopImpl *impl_;
};

} // namespace ss
