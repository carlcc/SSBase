//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "RefCounted.h"
#include <cstdint>

namespace ss
{

class Color
{
public:
private:
    union {
        struct
        {
            uint8_t a;
            uint8_t b;
            uint8_t g;
            uint8_t r;
        };
        uint32_t rgba;
    };
};

} // namespace ss