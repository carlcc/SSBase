//
// Created by carl on 20-4-4.
//
#include "Time.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace ss {

int64_t Time::CurrentTimeMillis()
{
    using namespace std::chrono;
    return (int64_t)duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int64_t Time::SteadyTimeMillis()
{
    using namespace std::chrono;
    return (int64_t)duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

void Time::SleepMillis(int64_t millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

void Time::InterruptableSleepMillis(int64_t millis)
{
    std::mutex mutex;
    std::condition_variable cv;

    std::unique_lock<std::mutex> lck(mutex);
    cv.wait_for(lck, std::chrono::milliseconds(millis));
}

}
