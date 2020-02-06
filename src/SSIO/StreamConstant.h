//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

class StreamConstant
{
public:
    enum ErrorCode
    {
        kOk = 0,
        kEof = -1,
        kUnknown = -128
    };
};