//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Assert.h"
#include "Ptr.h"
#include "Variant.h"
#include <string>

namespace ss
{

class Object;

class PropertyAccessor : public RefCounted
{
public:
    virtual void Get(const Object *obj, Variant &dest) const = 0;

    virtual void Set(Object *obj, const Variant &src) = 0;
};

template <class ClassType, class Getter, class Setter> class PropertyAccessorImpl : public PropertyAccessor
{
public:
    PropertyAccessorImpl(Getter getFunction, Setter setFunction) : getter_(getFunction), setter_(setFunction)
    {
    }

    void Get(const Object *ptr, Variant &value) const override
    {
        SSASSERT(ptr != nullptr);
        const auto classPtr = static_cast<const ClassType *>(ptr);
        getter_(*classPtr, value);
    }

    /// Invoke setter function.
    void Set(Object *ptr, const Variant &value) override
    {
        SSASSERT(ptr);
        auto classPtr = static_cast<ClassType *>(ptr);
        setter_(*classPtr, value);
    }

private:
    Getter getter_;
    Setter setter_;
};

template <class ClassType, class Getter, class Setter>
class SharedPtr<PropertyAccessor> MakePropertyAccessor(Getter getFunction, Setter setFunction)
{
    return SharedPtr<PropertyAccessor>(new PropertyAccessorImpl<ClassType, Getter, Setter>(getFunction, setFunction));
}

class Property
{
    enum Flag
    {
        kFlagNone = 0,
    };
    std::string name_;
    Variant::Type type;
    SharedPtr<PropertyAccessor> accessor_;
    int32_t flags;
};

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