//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "InputStream.h"

namespace ss
{

class SeekableInputStream : public InputStream
{
    SS_OBJECT(SeekableInputStream, InputStream)
public:
    enum Whence
    {
        kSeekCur,
        kSeekSet,
        kSeekEnd
    };

    // returns  StreamConstant::ErrorCode::kOk on success, or error code
    virtual int Seek(int64_t offset, Whence whence) = 0;
};

}