//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Object.h>
#include <SSBase/Ptr.h>
#include <iostream>
#include <string>

namespace TestRefCount
{

class Base : public ss::RefCounted
{
public:
    Base(const std::string &name)
    {
        name_ = name;
        std::cout << "Construct base of " << name << std::endl;
    }
    ~Base()
    {
        std::cout << "Destruct base of " << name_ << std::endl;
    }

    std::string name_;
};

class Child : public Base
{
public:
    Child(const std::string &name) : Base(name)
    {
        std::cout << "Construct child of " << name << std::endl;
    }
    ~Child()
    {
        std::cout << "Destruct child of " << name_ << std::endl;
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
        std::cout << "After block 1" << std::endl;
        {
            ss::SharedPtr<Child> c = new Child("OBject2");
            b = c;
            wb = b;

            std::cout << "b == c " << (b == c) << std::endl;
        }
        // compile error
        // ss::SharedPtr<D> d = new D;
        // b = d;

        {
            ss::SharedPtr<Base> spb = wb.Lock();
            std::cout << "spb is null? " << (spb == nullptr) << std::endl;
            std::cout << "spb is  " << spb->name_ << std::endl;
        }

        std::cout << "After block 2" << std::endl;

        b = new Child("Object3");

        std::cout << "After block 3" << std::endl;

        {

            ss::SharedPtr<Base> spb = wb.Lock();
            std::cout << "spb is null? " << (spb == nullptr) << std::endl;
        }
    }
}
}