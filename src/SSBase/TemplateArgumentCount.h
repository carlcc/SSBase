//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

namespace ss {

namespace internal {

    template <class Arg1, class... Args>
    struct ArgumentCount_ {
        static constexpr int Value = 1 + ArgumentCount_<Args...>::Value;
    };
    template <class Arg1>
    struct ArgumentCount_<Arg1> {
        static constexpr int Value = 1;
    };

    template <int N, class Arg1, class... Args>
    struct GetNthType_ {
        using Type = typename GetNthType_<N - 1, Args...>::Type;
    };
    template <class Arg1, class... Args>
    struct GetNthType_<0, Arg1, Args...> {
        using Type = Arg1;
    };
}

template <class... Args>
struct ArgumentCount {
    static constexpr int Value = internal::ArgumentCount_<Args...>::Value;
};
template <>
struct ArgumentCount<> {
    static constexpr int Value = 0;
};

template <int N, class... Args>
struct GetNthType {
    using Type = typename internal::GetNthType_<N, Args...>::Type;
};

}