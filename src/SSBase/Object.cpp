//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Object.h"

namespace ss
{

const char *Object::GetTypeNameStatic()
{
    return "Object";
}

const RuntimeTypeInfo *Object::GetTypeInfoStatic()
{
    static ss::RuntimeTypeInfo runtimeTypeInfo{"Object", nullptr, Object::GetAllPropertiesStatic};
    return &runtimeTypeInfo;
}

const PropertyMap &Object::GetAllPropertiesStatic()
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

const char *Object::GetTypeName()
{
    return Object::GetTypeNameStatic();
}

const ss::RuntimeTypeInfo *Object::GetTypeInfo()
{
    return Object::GetTypeInfoStatic();
}

const PropertyMap &Object::GetAllProperties()
{
    return Object::GetAllPropertiesStatic();
}

PropertyMap Object::RegisterProperties()
{
    PropertyMap propertyMap;
    return propertyMap;
}

void Object::DisconnectAll(const std::string &signal)
{
    signals_[signal].disconnect_all();
}

void Object::DisconnectAll()
{
    signals_.clear();
}

void Object::Emit(const std::string &signal, VariantMap &params)
{
    auto it = signals_.find(signal);
    if (it == signals_.end())
    {
        return;
    }
    it->second.emit(params);
}

} // namespace ss
