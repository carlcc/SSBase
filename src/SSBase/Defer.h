//
// Created by carl on 20-4-3.
//

#pragma once

#include <functional>
#include <utility>

#define DEFER_COMBINE1(X,Y) X##Y
#define DEFER_COMBINE(X,Y) DEFER_COMBINE1(X,Y)

#define OnScopeExit ::ss::Defer DEFER_COMBINE(_defer_, __LINE__) = [&]() -> void

namespace ss {

class Defer {
public:
    template <typename Callable>
    Defer(const Callable& defer) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : defered_(std::function<void()>(defer))
    {
    }

    ~Defer()
    {
        if (defered_) {
            defered_();
        }
    }

    void Cancel()
    {
        defered_ = nullptr;
    }

private:
    std::function<void()> defered_;
};

}