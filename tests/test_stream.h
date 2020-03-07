//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Ptr.h>
#include <SSIO/stream/BufferedInputStream.h>
#include <SSIO/stream/BufferedOutputStream.h>
#include <SSIO/stream/FileInputStream.h>
#include <SSIO/stream/FileOutputStream.h>

namespace TestStream
{

bool test(int argc, char **argv)
{
    using namespace ss;
    SharedPtr<InputStream> fis1 = MakeShared<FileInputStream>(argv[0]);
    SharedPtr<OutputStream> fos1 =
        MakeShared<FileOutputStream>(argv[0] + String(".copy"));

    auto fis = MakeShared<BufferedInputStream>(fis1);
    auto fos = MakeShared<BufferedOutputStream>(fos1);
    char buffer[4096];
    if (fis->IsValid() && fos->IsValid())
    {
        while (fis->Available() > 0)
        {
            auto c = fis->Read(buffer, sizeof(buffer));
            fos->Write(buffer, c);
        }
    }

    SharedPtr<FileInputStream> fis2 = MakeShared<FileInputStream>(__FILE__);
    std::string code = fis2->ReadAll();
    std::cout << code << std::endl;

    return true;
}

} // namespace TestStream