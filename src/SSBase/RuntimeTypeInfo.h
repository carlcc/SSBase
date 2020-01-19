//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Assert.h"
#include "Property.h"
#include <string>

namespace ss
{

using PropertyMap = std::map<std::string, Property>;

struct RuntimeTypeInfo
{
    const char *name;
    const RuntimeTypeInfo *parent;
    const PropertyMap &(*GetAllProperties)();
};

template <class T> struct RuntimeTypeInfoImpl : public RuntimeTypeInfo
{
};

} // namespace ss