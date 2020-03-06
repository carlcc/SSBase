//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//
#include <iostream>

#include "test_archive.h"
#include "test_filesystem.h"
#include "test_net.h"
#include "test_reflection.h"
#include "test_signal.h"
#include "test_stream.h"
#include "test_string.h"
#include "test_variant.h"
#include "testrefcounter.h"

int main()
{
    TestString::test();

    TestRefCount::test_refcounter();

    TestVariant::test();

    TestReflection::test();

    TestSignal::test();

    TestFileSystem::test();

    TestStream::test();

    TestArchive::test();

    TestNet::test();

    std::cout << "end main" << std::endl;

    return 0;
}