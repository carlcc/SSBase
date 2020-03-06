//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

namespace ss
{

template <class T> class Singleton
{
private:
    Singleton() = delete;

public:
    T &Instance()
    {
        static T t;
        return t;
    }
};

} // namespace ss