//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/ClassDB.h>
#include <SSBase/Object.h>

namespace TestReflection
{

class Child : public ss::Object
{
    SS_OBJECT(Child, ss::Object)

protected:
    static ss::PropertyMap RegisterProperties()
    {
        SS_BEGIN_REGISTER_PROPERTIES();
        SS_REGISTER_PROPERTY_GET_SET("IsEnabled", bool, &Child::IsEnabled, &Child::SetIsEnabled, 0);
        SS_END_REGISTER_PROPERTIES();
    }

public:
    bool IsEnabled() const
    {
        return isEnabled_;
    }

    void SetIsEnabled(bool isEnabled)
    {
        isEnabled_ = isEnabled;
    }

private:
    bool isEnabled_{false};
};

class Child2 : public Child
{
    SS_OBJECT(Child2, Child)
protected:
    static ss::PropertyMap RegisterProperties()
    {
        SS_BEGIN_REGISTER_PROPERTIES();

        SS_REGISTER_PROPERTY_GET_SET("IsVisible", bool, &Child2::IsVisible, &Child2::SetIsVisible, 0);
        SS_REGISTER_PROPERTY_GET_SET("Name", std::string, &Child2::GetName, &Child2::SetName, 0);

        SS_END_REGISTER_PROPERTIES();
    }

public:
    bool isVisible_;
    bool IsVisible() const
    {
        return isVisible_;
    }
    void SetIsVisible(bool isVisible)
    {
        isVisible_ = isVisible;
    }

    std::string name_;
    const std::string &GetName() const
    {
        return name_;
    }
    void SetName(const std::string &name)
    {
        name_ = name;
    }
};

class Child3 : public ss::Object
{
    SS_OBJECT(Child3, ss::Object)

public:
};

class Child4 : public ss::Object
{
    SS_OBJECT(Child4, ss::Object)

public:
    Child4()
    {
        std::cout << "Construct child4" << std::endl;
    }
    ~Child4()
    {
        std::cout << "Destruct child4" << std::endl;
    }
};

bool test()
{
    Child c;

    {
        auto &props = Child::GetAllPropertiesStatic();
        auto &prop = props.at("IsEnabled");
        ss::Variant v;
        c.SetIsEnabled(false);
        prop.GetAccessor()->Get(&c, v);
        SSASSERT(!v.GetBool());
        c.SetIsEnabled(true);
        prop.GetAccessor()->Get(&c, v);
        SSASSERT(v.GetBool());

        v = false;
        prop.GetAccessor()->Set(&c, v);
        SSASSERT(!c.IsEnabled());
        v = true;
        prop.GetAccessor()->Set(&c, v);
        SSASSERT(c.IsEnabled());
    }
    Child2 c2;
    {
        // inherit from Child
        auto &props = Child2::GetAllPropertiesStatic();
        auto &prop = props.at("IsEnabled");
        ss::Variant v;
        c2.SetIsEnabled(false);
        prop.GetAccessor()->Get(&c2, v);
        SSASSERT(!v.GetBool());
        c2.SetIsEnabled(true);
        prop.GetAccessor()->Get(&c2, v);
        SSASSERT(v.GetBool());

        v = false;
        prop.GetAccessor()->Set(&c2, v);
        SSASSERT(!c2.IsEnabled());
        v = true;
        prop.GetAccessor()->Set(&c2, v);
        SSASSERT(c2.IsEnabled());
    }
    {
        // new property
        auto &props = Child2::GetAllPropertiesStatic();
        auto &prop = props.at("IsVisible");
        ss::Variant v;
        c2.SetIsVisible(false);
        prop.GetAccessor()->Get(&c2, v);
        SSASSERT(!v.GetBool());
        c2.SetIsVisible(true);
        prop.GetAccessor()->Get(&c2, v);
        SSASSERT(v.GetBool());

        v = false;
        prop.GetAccessor()->Set(&c2, v);
        SSASSERT(!c2.IsVisible());
        v = true;
        prop.GetAccessor()->Set(&c2, v);
        SSASSERT(c2.IsVisible());
    }
    {
        // new property
        auto &props = Child2::GetAllPropertiesStatic();
        auto &prop = props.at("Name");
        ss::Variant v;
        std::string name = "hello, world";
        c2.SetName(name);
        prop.GetAccessor()->Get(&c2, v);
        SSASSERT(v.GetString() == name);
        c2.SetName("true");
        prop.GetAccessor()->Get(&c2, v);
        SSASSERT(v.GetString() == "true");

        v = "nice";
        prop.GetAccessor()->Set(&c2, v);
        SSASSERT(c2.GetName() == "nice");
        v = "true";
        prop.GetAccessor()->Set(&c2, v);
        SSASSERT(c2.GetName() == "true");
    }
    Child3 c3;
    SSASSERT(c.IsInstanceOf<ss::Object>());
    SSASSERT(c2.IsInstanceOf<Child>());
    SSASSERT(c2.IsInstanceOf<Child2>());
    SSASSERT(!c3.IsInstanceOf<Child>());
    SSASSERT(!c3.IsInstanceOf<Child2>());
    SSASSERT(c3.IsInstanceOf<Child3>());
    SSASSERT(c3.IsInstanceOf<ss::Object>());

    ss::Object *obj = &c3;
    SSASSERT(ss::Object::DynamicCast<Child>(obj) == nullptr);
    SSASSERT(ss::Object::DynamicCast<Child3>(obj) == &c3);

    ss::ClassDB::RegisterConstructor<Child4>();
    ss::ClassDB::Construct<Child4>();

    return true;
}

} // namespace TestReflection