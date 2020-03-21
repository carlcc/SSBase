//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "ClassDB.h"
#include <unordered_map>

namespace ss {

class ClassDBImpl {
    struct StringHasher {
        size_t operator()(const String& s) const
        {
            return size_t(s.Hash());
        }
    };
    using CtorMap = std::unordered_map<String, ClassDB::ObjectCtor, StringHasher>;
    using CategoryMap = std::unordered_map<String, CtorMap, StringHasher>;

public:
    static ClassDBImpl& Get()
    {
        static ClassDBImpl impl;
        return impl;
    }

    void RegisterObjectCtor(const String& className, ClassDB::ObjectCtor constructor)
    {
        objectCtorMap_.insert({ className, constructor });
    }

    void RegisterObjectCtor(const String& className, ClassDB::ObjectCtor constructor, const String& category)
    {
        objectCtorMap_.insert({ className, constructor });
        categoryMap_[category][className] = constructor;
    }

    ClassDB::ObjectCtor GetObjectCtor(const String& className)
    {
        auto it = objectCtorMap_.find(className);
        if (it == objectCtorMap_.end()) {
            return nullptr;
        }
        return it->second;
    }

private:
    CtorMap objectCtorMap_;
    CategoryMap categoryMap_;
};

void ClassDB::RegisterObjectCtor(const String& className, ClassDB::ObjectCtor constructor)
{
    ClassDBImpl::Get().RegisterObjectCtor(className, constructor);
}

void ClassDB::RegisterObjectCtor(const String& className, ClassDB::ObjectCtor constructor, const String& category)
{
    ClassDBImpl::Get().RegisterObjectCtor(className, constructor, category);
}

ClassDB::ObjectCtor ClassDB::GetObjectCtor(const String& className)
{
    return ClassDBImpl::Get().GetObjectCtor(className);
}

} // namespace ss
