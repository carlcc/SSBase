//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "testrefcounter.h"
#include "test_reflection.h"
#include "test_variant.h"

int main()
{
//    TestRefCount::test_refcounter();

    TestReflection::test();

    TestVariant::test();

    std::cout << "end main" << std::endl;

}