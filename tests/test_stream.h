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
        }
    }

    SharedPtr<FileInputStream> fis2 = MakeShared<FileInputStream>(__FILE__);
    std::string code = fis2->ReadAll();
    std::cout << code << std::endl;

    return true;
}

}