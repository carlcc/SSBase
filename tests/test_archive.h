//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSIO/Archive.h>
#include <SSIO/InputStream.h>
#include <SSIO/StreamConstant.h>

namespace TestArchive
{
bool test()
{
    using namespace ss;
    String path = __FILE__;
    StringView path2 = path.SubStringView(0, path.RFind("/"));
    Archive archive(path2 + "/a.zip");

    auto &entries = archive.GetEntries();
    for (auto &e : entries)
    {
        std::cout << "== " << e << std::endl;
    }
    auto is = archive.GetInputStream("include/zipconf.h");
    std::cout << "== " << is->IsValid() << std::endl;
    char buf[256];
    std::string result;
    while (true)
    {
        auto ret = is->Read(buf, 256);
        if (ret == StreamConstant::ErrorCode::kEof)
        {
            break;
        }
        if (ret == StreamConstant::ErrorCode::kUnknown)
        {
            SSASSERT(false);
        }

        result += std::string(buf, ret);
    }
    std::string kResult = R"(#ifndef _HAD_ZIPCONF_H
#define _HAD_ZIPCONF_H

/*
   zipconf.h -- platform specific include file

   This file was generated automatically by CMake
   based on ../cmake-zipconf.h.in.
 */

#define LIBZIP_VERSION "1.6.1"
#define LIBZIP_VERSION_MAJOR 1
#define LIBZIP_VERSION_MINOR 6
#define LIBZIP_VERSION_MICRO 1

/* #undef ZIP_STATIC */



#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>

typedef int8_t zip_int8_t;
typedef uint8_t zip_uint8_t;
typedef int16_t zip_int16_t;
typedef uint16_t zip_uint16_t;
typedef int32_t zip_int32_t;
typedef uint32_t zip_uint32_t;
typedef int64_t zip_int64_t;
typedef uint64_t zip_uint64_t;

#define ZIP_INT8_MIN	 (-ZIP_INT8_MAX-1)
#define ZIP_INT8_MAX	 0x7f
#define ZIP_UINT8_MAX	 0xff

#define ZIP_INT16_MIN	 (-ZIP_INT16_MAX-1)
#define ZIP_INT16_MAX	 0x7fff
#define ZIP_UINT16_MAX	 0xffff

#define ZIP_INT32_MIN	 (-ZIP_INT32_MAX-1L)
#define ZIP_INT32_MAX	 0x7fffffffL
#define ZIP_UINT32_MAX	 0xffffffffLU

#define ZIP_INT64_MIN	 (-ZIP_INT64_MAX-1LL)
#define ZIP_INT64_MAX	 0x7fffffffffffffffLL
#define ZIP_UINT64_MAX	 0xffffffffffffffffULL

#endif /* zipconf.h */
)";
    SSASSERT(result == kResult);
    return true;
}
}