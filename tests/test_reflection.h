//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Object.h>

namespace TestReflection
{

class Child : public ss::Object
{
    SS_OBJECT(Child, ss::Object)

public:
};

class Child2 : public Child
{
    SS_OBJECT(Child2, Child)

public:
};

class Child3 : public ss::Object
{
SS_OBJECT(Child3, ss::Object)

public:
};

bool test()
{
    Child c;
    Child2 c2;
    Child3 c3;
    SSASSERT(c.IsInstanceOf<ss::Object>());
    SSASSERT(c2.IsInstanceOf<Child>());
    SSASSERT(c2.IsInstanceOf<Child2>());
    SSASSERT(!c3.IsInstanceOf<Child>());
    SSASSERT(!c3.IsInstanceOf<Child2>());
    SSASSERT(c3.IsInstanceOf<Child3>());
    SSASSERT(c3.IsInstanceOf<ss::Object>());

    return true;
}

}