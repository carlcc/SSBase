//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "EndPoint.h"
#include "../SSBase/Convert.h"
#include "EndPointInternal.h"

namespace ss
{

EndPoint::EndPoint() : impl_(nullptr)
{
}

EndPoint::EndPoint(const CharSequence &ip, uint16_t port) : impl_(nullptr)
{
    InitWithIpAndPort(ip, port);
}

EndPoint::EndPoint(const String &IP, uint16_t port) : impl_(nullptr)
{
    InitWithIpAndPort(IP, port);
}

EndPoint::EndPoint(const CharSequence &ipAndPort) : impl_(nullptr)
{
    InitWithString(ipAndPort);
}

EndPoint::EndPoint(const String &IPAndPort) : impl_(nullptr)
{
    InitWithString(IPAndPort);
}

EndPoint::EndPoint(const EndPoint &ep) : impl_(nullptr)
{
    *this = ep;
}

EndPoint::EndPoint(EndPoint &&ep) noexcept : impl_(nullptr)
{
    impl_ = std::move(ep.impl_);
}

EndPoint::~EndPoint()
{
    impl_ = nullptr;
}

EndPoint &EndPoint::operator=(const EndPoint &ep)
{
    if (&ep == this)
    {
        return *this;
    }

    if (ep.impl_ != nullptr)
    {
        if (impl_ == nullptr)
        {
            impl_ = std::make_unique<Impl>();
        }
        *impl_ = *ep.impl_;
    }
    else
    {
        impl_ = nullptr;
    }

    return *this;
}

EndPoint &EndPoint::operator=(EndPoint &&ep) noexcept
{
    impl_ = std::move(ep.impl_);
    return *this;
}

bool EndPoint::IsValid() const
{
    return Port() != 0;
}

bool EndPoint::IsIPV4() const
{
    return IsValid() && impl_->addr4_.sin_family == AF_INET;
}

bool EndPoint::IsIPV6() const
{
    return IsValid() && impl_->addr6_.sin6_family == AF_INET6;
}

String EndPoint::IP() const
{
    char buf[128] = {0};

    if (impl_ != nullptr)
    {
        switch (impl_->addr4_.sin_family)
        {
        case AF_INET6: {
            auto *addr = reinterpret_cast<sockaddr_in6 *>(&impl_->addr6_);
            if (0 != uv_ip6_name(addr, buf, sizeof(buf)))
            {
                buf[0] = '\0';
            }
            break;
        }
        case AF_INET:
        default: {
            auto *addr = reinterpret_cast<sockaddr_in *>(&impl_->addr4_);
            if (0 != uv_ip4_name(addr, buf, sizeof(buf)))
            {
                buf[0] = '\0';
            }
            break;
        }
        }
    }
    return buf;
}

uint16_t EndPoint::Port() const
{
    if (impl_ == nullptr)
    {
        return 0;
    }
    uint16_t port;
    switch (impl_->addr4_.sin_family)
    {
    case AF_INET6: {
        auto *addr = reinterpret_cast<sockaddr_in6 *>(&impl_->addr6_);
        port = addr->sin6_port;
        break;
    }
    case AF_INET:
    default: {
        auto *addr = reinterpret_cast<sockaddr_in *>(&impl_->addr4_);
        port = addr->sin_port;
        break;
    }
    }
    return htons(port);
}

void EndPoint::InitWithIpAndPort(const CharSequence &ip, uint16_t port)
{
    impl_ = std::make_unique<Impl>();
    if (ip.Contains("."))
    {
        auto *addr = reinterpret_cast<sockaddr_in *>(&impl_->addr4_);
        if (0 != uv_ip4_addr(ip.ToStdString().c_str(), port, addr))
        {
            impl_ = nullptr;
        }
    }
    else
    {
        auto *addr = reinterpret_cast<sockaddr_in6 *>(&impl_->addr6_);
        if (0 != uv_ip6_addr(ip.ToStdString().c_str(), port, addr))
        {
            impl_ = nullptr;
        }
    }
    if (Port() == 0)
    {
        impl_ = nullptr;
    }
}

void EndPoint::InitWithString(const CharSequence &ipAndPort)
{
    if (ipAndPort.Contains("."))
    {
        auto index = ipAndPort.RFind(":");
        if (index == CharSequence::kNPos)
        {
            return;
        }
        StringView ip = ipAndPort.SubStringView(0, index);
        auto port = Convert::StringTo<uint16_t>(ipAndPort.SubString(index + 1));
        InitWithIpAndPort(ip, port);
    }
    else
    {
        auto startIndex = ipAndPort.Find("[");
        if (startIndex == CharSequence::kNPos)
        {
            return;
        }
        auto endIndex = ipAndPort.Find("]:", startIndex + 1);
        if (endIndex == CharSequence::kNPos)
        {
            return;
        }
        StringView ip = ipAndPort.SubStringView(startIndex + 1, endIndex - startIndex - 1);
        auto port = Convert::StringTo<int16_t>(ipAndPort.SubStringView(endIndex + 2));
        InitWithIpAndPort(ip, port);
    }
}

} // namespace ss