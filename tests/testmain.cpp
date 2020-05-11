//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//
#include "test_archive.h"
#include "test_filesystem.h"
#include "test_net.h"
#include "test_stream.h"
#include "test_string.h"
#include "testrefcounter.h"
#include <iostream>

int main(int argc, char** argv)
{
    TestString::test();

    TestRefCount::test_refcounter();

    TestFileSystem::test();

    TestStream::test(argc, argv);

    TestArchive::test();

    TestNet::test();

    std::cout << "end main" << std::endl;

    return 0;
}