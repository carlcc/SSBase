//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//
#include <iostream>

#include "test_reflection.h"
#include "test_signal.h"
#include "test_stream.h"
#include "test_string.h"
#include "test_variant.h"
#include "testrefcounter.h"

int main()
{
    TestString::test();

    //    TestRefCount::test_refcounter();

    // TestReflection::test();

    //    TestVariant::test();

    // TestSignal::test();

    // TestStream::test();

    std::cout << "end main" << std::endl;
}