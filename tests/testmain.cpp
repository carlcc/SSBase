//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "testrefcounter.h"
#include "test_reflection.h"

int main()
{
    TestRefCount::test_refcounter();

    TestReflection::test();

    std::cout << "end main" << std::endl;

}