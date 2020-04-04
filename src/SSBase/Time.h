//
// Created by carl on 20-4-4.
//

#pragma once

#include <cstdint>

namespace ss {

class Time {
public:

    static int64_t CurrentTimeMillis();

    static int64_t SteadyTimeMillis();

    static void SleepMillis(int64_t millis);

    static void InterruptableSleepMillis(int64_t millis);
};

}
