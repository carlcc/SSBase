//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once
#include <SSBase/ClassDB.h>
#include <SSBase/Object.h>
#include <SSBase/Ptr.h>
#include <sstream>

namespace TestVariant {

std::stringstream gOutput;

class A : public ss::RefCounted {
public:
    A()
    {
        gOutput << "Construct A" << std::endl;
    }
    ~A()
    {
        gOutput << "Destruct A" << std::endl;
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
            gOutput << intValue.GetInt() << std::endl;
            pa = sharedPtrValue.GetPtr();
        }
        gOutput << "Exit scope" << std::endl;
    }

    gOutput << int64Value.GetInt64() << std::endl;
    gOutput << stringValue.GetString() << std::endl;

    std::string kResult = R"(Construct A
1
Exit scope
Destruct A
1
hello
)";
    SSASSERT(kResult == gOutput.str());
    return true;
}

} // namespace TestVariant