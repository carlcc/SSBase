//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>

#define SSASSERT(condition)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            printf("%s:%d: %s got result false. ", __FILE__, __LINE__, #condition);                                    \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (false)

#define SSASSERT2(condition, msg, ...)                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            printf("%s:%d: %s got result false. " msg, __FILE__, __LINE__, #condition, ##__VA_ARGS__);                 \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (false)
