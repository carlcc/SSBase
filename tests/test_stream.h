//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Ptr.h>
#include <SSIO/FileInputStream.h>
#include <SSIO/FileOutputStream.h>

namespace TestStream
{

bool test()
{
    using namespace ss;
    SharedPtr<InputStream> fis = MakeShared<FileInputStream>("/Users/chenchen02/all.tar.bz2");
    SharedPtr<OutputStream> fos = MakeShared<FileOutputStream>("/Users/chenchen02/all.tar.bz2" + std::string(".copy"));
    char buffer[4096];
    if (fis->IsValid() && fos->IsValid())
    {
        while (fis->Available() > 0)
        {
            auto c = fis->Read(buffer, sizeof(buffer));
            fos->Write(buffer, c);
            // std::cout << std::string(buffer, c);
        }
    }

    return true;
}

}