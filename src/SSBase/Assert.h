//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>

#define SSASSERT(condition)                                                                                            \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        printf("%s:%d: %s got result false. ", __FILE__, __LINE__, #condition);                                        \
        abort();                                                                                                       \
    }

#define SSASSERT2(condition, msg, ...)                                                                                 \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        printf("%s:%d: %s got result false. " msg, __FILE__, __LINE__, #condition, ##__VA_ARGS__);                     \
        abort();                                                                                                       \
    }
