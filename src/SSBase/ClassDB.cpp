//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "ClassDB.h"
#include <string>
#include <unordered_map>

namespace ss
{

class ClassDBImpl
{
    using CtorMap = std::unordered_map<std::string, ClassDB::ObjectCtor>;
    using CategoryMap = std::unordered_map<std::string, CtorMap>;

public:
    static ClassDBImpl &Get()
    {
        static ClassDBImpl impl;
        return impl;
    }

    void RegisterObjectCtor(const std::string &className, ClassDB::ObjectCtor constructor)
    {
        objectCtorMap_.insert({className, constructor});
    }

    void RegisterObjectCtor(const std::string &className, ClassDB::ObjectCtor constructor, const std::string &category)
    {
        objectCtorMap_.insert({className, constructor});
        categoryMap_[category][className] = constructor;
    }

    ClassDB::ObjectCtor GetObjectCtor(const std::string &className)
    {
        auto it = objectCtorMap_.find(className);
        if (it == objectCtorMap_.end())
        {
            return nullptr;
        }
        return it->second;
    }

private:
    CtorMap objectCtorMap_;
    CategoryMap categoryMap_;
};

void ClassDB::RegisterObjectCtor(const std::string &className, ClassDB::ObjectCtor constructor)
{
    ClassDBImpl::Get().RegisterObjectCtor(className, constructor);
}

void ClassDB::RegisterObjectCtor(const std::string &className, ClassDB::ObjectCtor constructor,
                                 const std::string &category)
{
    ClassDBImpl::Get().RegisterObjectCtor(className, constructor, category);
}

ClassDB::ObjectCtor ClassDB::GetObjectCtor(const std::string &className)
{
    return ClassDBImpl::Get().GetObjectCtor(className);
}

} // namespace ss
