//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Object.h>
#include <SSBase/Ptr.h>
#include <iostream>
#include <sstream>
#include <string>

namespace TestRefCount
{

std::stringstream gOutput;
class Base : public ss::RefCounted
{
public:
    Base(const std::string &name)
    {
        name_ = name;
        gOutput << "Construct base of " << name << std::endl;
    }
    ~Base()
    {
        gOutput << "Destruct base of " << name_ << std::endl;
    }

    std::string name_;
};

class Child : public Base
{
public:
    Child(const std::string &name) : Base(name)
    {
        gOutput << "Construct child of " << name << std::endl;
    }
    ~Child()
    {
        gOutput << "Destruct child of " << name_ << std::endl;
    }
};

class D : public ss::RefCounted
{
};

void test_refcounter()
{
    {
        ss::WeakPtr<Base> wb;
        ss::SharedPtr<Base> b;
        {
            ss::SharedPtr<Child> c = new Child("OBject1");
        }
        gOutput << "After block 1" << std::endl;
        {
            ss::SharedPtr<Child> c = new Child("OBject2");
            b = c;
            wb = b;

            gOutput << "b == c " << (b == c) << std::endl;
        }
        // compile error
        // ss::SharedPtr<D> d = new D;
        // b = d;

        {
            ss::SharedPtr<Base> spb = wb.Lock();
            gOutput << "spb is null? " << (spb == nullptr) << std::endl;
            gOutput << "spb is  " << spb->name_ << std::endl;
        }

        gOutput << "After block 2" << std::endl;

        b = new Child("Object3");

        gOutput << "After block 3" << std::endl;

        {

            ss::SharedPtr<Base> spb = wb.Lock();
            gOutput << "spb is null? " << (spb == nullptr) << std::endl;
        }
    }

    std::string kResult = R"(Construct base of OBject1
Construct child of OBject1
Destruct child of OBject1
Destruct base of OBject1
After block 1
Construct base of OBject2
Construct child of OBject2
b == c 1
spb is null? 0
spb is  OBject2
After block 2
Construct base of Object3
Construct child of Object3
Destruct child of OBject2
Destruct base of OBject2
After block 3
spb is null? 1
Destruct child of Object3
Destruct base of Object3
)";
    std::string result = gOutput.str();
    SSASSERT(result == kResult);
}
}