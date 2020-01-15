//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once
#include <SSBase/ClassDB.h>
#include <SSBase/Object.h>
#include <SSBase/Ptr.h>

namespace TestVariant
{

class A : public ss::RefCounted
{
public:
    A()
    {
        std::cout << "Construct A " << std::endl;
    }
    ~A()
    {
        std::cout << "Destruct A " << std::endl;
    }
};
bool test()
{
    using namespace ss;

    Variant intValue = 1;
    Variant int64Value = int64_t(1);
    Variant stringValue = "hello";
    {
        SharedPtr<A> pa;
        {
            Variant sharedPtrValue = SharedPtr<A>(new A);
            std::cout << intValue.GetInt() << std::endl;
            pa = sharedPtrValue.GetPtr();
        }
        std::cout << "Exit scope" << std::endl;
    }

    std::cout << int64Value.GetInt64() << std::endl;
    std::cout << stringValue.GetString() << std::endl;
    return true;
}

}