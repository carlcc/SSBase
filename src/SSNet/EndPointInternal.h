//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "EndPoint.h"
#include <uv.h>

namespace ss {

struct EndPoint::Impl {
    union {
        sockaddr addr_;
        sockaddr_in6 addr6_;
        sockaddr_in addr4_;
    };
};

} // namespace ss