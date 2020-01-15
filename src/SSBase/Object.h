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
    }

class Object : public RefCounted
{
public:
    static const char *GetTypeNameStatic()
    {
        return "Object";
    }

    static const RuntimeTypeInfo *GetTypeInfoStatic()
    {
        static ss::RuntimeTypeInfo runtimeTypeInfo{"Object", nullptr};
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