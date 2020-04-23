//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#define SSASSERT(condition)                                                           \
    do {                                                                              \
        if (!(condition)) {                                                           \
            printf("%s:%d: %s got result false. \n", __FILE__, __LINE__, #condition); \
            fflush(stdout);                                                           \
            throw std::runtime_error(#condition);                                     \
        }                                                                             \
    } while (false)

#define SSASSERT2(condition, msg, ...)                                                                   \
    do {                                                                                                 \
        if (!(condition)) {                                                                              \
            printf("%s:%d: %s got result false. \n" msg, __FILE__, __LINE__, #condition, ##__VA_ARGS__); \
            fflush(stdout);                                                                              \
            throw std::runtime_error(#condition);                                                        \
        }                                                                                                \
    } while (false)
