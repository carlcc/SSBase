//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "testrefcounter.h"
#include "test_reflection.h"
#include "test_variant.h"
#include "test_signal.h"
#include "test_stream.h"
int main()
{
//    TestRefCount::test_refcounter();

    TestReflection::test();

//    TestVariant::test();

    TestSignal::test();

    TestStream::test();

    std::cout << "end main" << std::endl;

}