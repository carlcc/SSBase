//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/ClassDB.h>
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



class Child4 : public ss::Object
{
SS_OBJECT(Child4, ss::Object)

public:
    Child4()
    {
        std::cout << "Construct child4" << std::endl;
    }
    ~Child4()
    {
        std::cout << "Destruct child4" << std::endl;
    }
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

    ss::ClassDB::RegisterConstructor<Child4>();
    ss::ClassDB::Construct<Child4>();

    return true;
}

} // namespace TestReflection