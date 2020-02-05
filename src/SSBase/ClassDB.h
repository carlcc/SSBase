//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Object.h"
#include "Ptr.h"
#include "Str.h"

namespace ss
{

class ClassDB
{
public:
    template <class T> using Ctor = SharedPtr<T> (*)();
    using ObjectCtor = Ctor<Object>;

    template <class T> class Constructor
    {
    public:
        explicit Constructor(ObjectCtor ctor) : ctor_(ctor)
        {
        }

        SharedPtr<T> operator()()
        {
            return SharedPtr<T>(ctor_());
        }

    private:
        ObjectCtor ctor_;
    };

    template <class T> static Constructor<T> GetConstructor()
    {
        return Constructor<T>(GetObjectCtor(T::GetTypeNameStatic()));
    }

    template <class T> static SharedPtr<T> Construct()
    {
        Constructor<T> constructor = GetConstructor<T>();
        return constructor();
    }

    template <class T> static void RegisterConstructor()
    {
        RegisterObjectCtor(T::GetTypeNameStatic(), []() -> SharedPtr<Object> { return SharedPtr<Object>(new T); });
    }

    template <class T> static void RegisterConstructor(const String &category)
    {
        RegisterObjectCtor(
            T::GetTypeNameStatic(), []() -> SharedPtr<Object> { return SharedPtr<Object>(new T); }, category);
    }

private:
    static void RegisterObjectCtor(const String &className, ObjectCtor constructor);

    static void RegisterObjectCtor(const String &className, ObjectCtor constructor, const String &category);

    static ObjectCtor GetObjectCtor(const String &className);

private:
};

} // namespace ss