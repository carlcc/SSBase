//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Assert.h"
#include "Property.h"
#include "Str.h"

namespace ss
{

using PropertyMap = std::map<String, Property>;

struct RuntimeTypeInfo
{
    const String &name;
    const RuntimeTypeInfo *parent;
    const PropertyMap &(*GetAllProperties)();
};

template <class T> struct RuntimeTypeInfoImpl : public RuntimeTypeInfo
{
};

} // namespace ss