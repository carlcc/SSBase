//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../SSBase/Str.h"
#include <cstdint>
#include <memory>

namespace ss
{

class EndPoint
{
public:
    EndPoint();
    EndPoint(const CharSequence &IP, uint16_t port);
    EndPoint(const String &IP, uint16_t port);
    EndPoint(const String &IPAndPort);       // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    EndPoint(const CharSequence &IPAndPort); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    EndPoint(const EndPoint &ep);
    EndPoint(EndPoint &&ep) noexcept;
    ~EndPoint();
    EndPoint &operator=(const EndPoint &ep);
    EndPoint &operator=(EndPoint &&ep) noexcept;

    bool IsValid() const;

    bool IsIPV4() const;

    bool IsIPV6() const;

    String IP() const;

    uint16_t Port() const;

private:
    void InitWithIpAndPort(const CharSequence &IP, uint16_t port);
    void InitWithString(const CharSequence &IPAndPort);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    friend class AsyncTcpSocketImpl;
    friend class TcpSocketImpl;
};

} // namespace ss