//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "RefCounted.h"
#include "RuntimeTypeInfo.h"
#include "Variant.h"
#include "thirdparty/sigslot.h"

namespace ss
{

#define SS_OBJECT(clazz, base)                                                                                         \
public:                                                                                                                \
    static const char *GetTypeNameStatic()                                                                             \
    {                                                                                                                  \
        return #clazz;                                                                                                 \
    }                                                                                                                  \
    static const ss::RuntimeTypeInfo *GetTypeInfoStatic()                                                              \
    {                                                                                                                  \
        static ss::RuntimeTypeInfo runtimeTypeInfo{#clazz, base::GetTypeInfoStatic()};                                 \
        return &runtimeTypeInfo;                                                                                       \
    }                                                                                                                  \
    const char *GetTypeName() override                                                                                 \
    {                                                                                                                  \
        return clazz::GetTypeNameStatic();                                                                             \
    }                                                                                                                  \
    const ss::RuntimeTypeInfo *GetTypeInfo() override                                                                  \
    {                                                                                                                  \
        return clazz::GetTypeInfoStatic();                                                                             \
    }                                                                                                                  \
    static const ss::PropertyMap &GetAllProperties()                                                                   \
    {                                                                                                                  \
        static struct PropertyMapLoader                                                                                \
        {                                                                                                              \
            PropertyMapLoader() : propertyMap_(clazz::RegisterProperties())                                            \
            {                                                                                                          \
            }                                                                                                          \
            ss::PropertyMap propertyMap_;                                                                              \
        } sPropertyMap;                                                                                                \
        return sPropertyMap.propertyMap_;                                                                              \
    }

#define SS_BEGIN_REGISTER_PROPERTIES() ss::PropertyMap propertyMap

#define SS_END_REGISTER_PROPERTIES() return propertyMap

#define SS_REGISTER_PROPERTY_GET_SET(propName, propType, propGetter, propSetter, flags)                                \
    do                                                                                                                 \
    {                                                                                                                  \
        SharedPtr<PropertyAccessor> accessor = ss::MakePropertyAccessor(propType, propGetter, propSetter);             \
        propertyMap.insert({propName, Variant::GetVariantType<propType>(), accessor, flags});                          \
    } while (false)

class Object : public RefCounted
{
public:
    static const char *GetTypeNameStatic()
    {
        return "Object";
    }

    static const RuntimeTypeInfo *GetTypeInfoStatic()
    {
        static ss::RuntimeTypeInfo runtimeTypeInfo{"Object", nullptr, Object::GetAllProperties};
        return &runtimeTypeInfo;
    }

    virtual const char *GetTypeName()
    {
        return Object::GetTypeNameStatic();
    }

    virtual const ss::RuntimeTypeInfo *GetTypeInfo()
    {
        return Object::GetTypeInfoStatic();
    }

    static const PropertyMap &GetAllProperties()
    {
        static struct PropertyMapLoader
        {
            PropertyMapLoader() : propertyMap_(Object::RegisterProperties())
            {
            }
            std::map<std::string, Property> propertyMap_;
        } sPropertyMap;
        return sPropertyMap.propertyMap_;
    }

protected:
    static PropertyMap RegisterProperties()
    {
        SS_BEGIN_REGISTER_PROPERTIES();
        SS_END_REGISTER_PROPERTIES();
    }

public:
    template <class clazz, class T> static bool IsInstanceOf(T *p)
    {
        const RuntimeTypeInfo *rti = p->GetTypeInfo();
        const RuntimeTypeInfo *targetRti = clazz::GetTypeInfoStatic();
        while (rti != nullptr)
        {
            if (rti == targetRti)
            {
                return true;
            }
            rti = rti->parent;
        }
        return false;
    }

    template <class clazz> bool IsInstanceOf()
    {
        return Object::IsInstanceOf<clazz>(this);
    }

    template <class T> static T *DynamicCast(Object *o)
    {
        if (Object::IsInstanceOf<T>(o))
        {
            return static_cast<T *>(o);
        }
        return nullptr;
    }

public:
private:
    using Signal = sigslot::signal1<Variant>;
    Signal signals;
};

} // namespace ss