//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "RefCounted.h"
#include "RuntimeTypeInfo.h"
#include "Variant.h"
#include "thirdparty/sigslot.h"
#include <map>

namespace ss
{

#define SS_OBJECT(clazz, base)                                                                                         \
private:                                                                                                               \
    using This = clazz;                                                                                                \
    using Super = base;                                                                                                \
                                                                                                                       \
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
    static const ss::PropertyMap &GetAllPropertiesStatic()                                                             \
    {                                                                                                                  \
        static struct PropertyMapLoader                                                                                \
        {                                                                                                              \
            PropertyMapLoader() : propertyMap_(clazz::RegisterProperties())                                            \
            {                                                                                                          \
            }                                                                                                          \
            ss::PropertyMap propertyMap_;                                                                              \
        } sPropertyMap;                                                                                                \
        return sPropertyMap.propertyMap_;                                                                              \
    }                                                                                                                  \
    const char *GetTypeName() override                                                                                 \
    {                                                                                                                  \
        return clazz::GetTypeNameStatic();                                                                             \
    }                                                                                                                  \
    const ss::RuntimeTypeInfo *GetTypeInfo() override                                                                  \
    {                                                                                                                  \
        return clazz::GetTypeInfoStatic();                                                                             \
    }                                                                                                                  \
    virtual const ss::PropertyMap &GetAllProperties() override                                                         \
    {                                                                                                                  \
        return clazz::GetAllPropertiesStatic();                                                                        \
    }

#define SS_BEGIN_REGISTER_PROPERTIES() ss::PropertyMap propertyMap = Super::GetAllPropertiesStatic()

#define SS_END_REGISTER_PROPERTIES() return propertyMap

#define SS_REGISTER_PROPERTY_GET_SET(propName, propType, propGetter, propSetter, flags)                                \
    do                                                                                                                 \
    {                                                                                                                  \
        auto accessor = ss::MakePropertyAccessor<This>(                                                                \
            [](const This &obj, ss::Variant &v) { v = (obj.*propGetter)(); },                                          \
            [](This &obj, const ss::Variant &v) { (obj.*propSetter)(ss::Variant::Getter<propType>::Get(v)); });        \
        propertyMap.insert({propName, ss::Property{propName, ss::GetVariantType<propType>(), accessor, 0}});           \
    } while (false)

class Object : public RefCounted, public sigslot::has_slots<>
{
public:
    static const char *GetTypeNameStatic();

    static const RuntimeTypeInfo *GetTypeInfoStatic();

    static const PropertyMap &GetAllPropertiesStatic();

    virtual const char *GetTypeName();

    virtual const ss::RuntimeTypeInfo *GetTypeInfo();

    virtual const PropertyMap &GetAllProperties();

protected:
    static PropertyMap RegisterProperties();

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
    using Signal = sigslot::signal1<VariantMap &>;
    using SignalMap = std::map<std::string, Signal>;

    const SignalMap &GetAllSignals() const
    {
        return signals_;
    }

    template <class T> void Connect(const std::string &signal, T *obj, void (T::*handler)(VariantMap &))
    {
        signals_[signal].connect(obj, handler);
    }

    template <class T> void Connect(const std::string &signal, SharedPtr<T> obj, void (T::*handler)(VariantMap &))
    {
        signals_[signal].connect(obj.Get(), handler);
    }

    template <class T> void Disconnect(const std::string &signal, T *obj)
    {
        signals_[signal].disconnect(obj);
    }

    template <class T> void Disconnect(const std::string &signal, SharedPtr<T> obj)
    {
        signals_[signal].disconnect(obj.Get());
    }

    void DisconnectAll(const std::string &signal);

    void DisconnectAll();

    void Emit(const std::string &signal, VariantMap &params);

    // TODO: how to manage the dummy object's life cycle?
    // void Connect(const std::string &signal, std::function<void(VariantMap &)> &&handler)
    // {
    //     struct Dummy : public sigslot::has_slots<>
    //     {
    //         explicit Dummy(std::function<void(VariantMap &)> &&handler)
    //             : handler_(std::forward<std::function<void(VariantMap &)>>(handler))
    //         {
    //         }
    //
    //         void handle(VariantMap &params)
    //         {
    //             handler_(params);
    //         }
    //
    //         std::function<void(VariantMap &)> handler_;
    //     };
    //     Connect(signal, new Dummy(std::forward<std::function<void(VariantMap &)>>(handler)), &Dummy::handle);
    // }

private:
    SignalMap signals_;
};

} // namespace ss