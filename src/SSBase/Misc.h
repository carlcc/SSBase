//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <cstdint>

namespace ss
{

class Misc
{
public:
    inline static uint32_t CeilToPowerOfTwo(uint32_t len)
    {
        uint32_t last = len;
        len &= len - 1;
        if (len == 0)
        {
            return last; // if len == 0 or len is 2's power, then return n
        }
        do
        {
            last = len;
            len &= len - 1;
        } while (len > 0);
        return last << 1u;
    }
};

};