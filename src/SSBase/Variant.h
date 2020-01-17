//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "Color.h"
#include "LinearMath.h"
#include "Ptr.h"

namespace ss
{

class Variant
{
public:
    using String = std::string;
    using Map = std::map<std::string, Variant>;
    using Vector = std::vector<Variant>;

private:
    union Value {
        int int_;
        int64_t int64_;
        float float_;
        double double_;
        bool bool_;
        String string_;
        Color color_;
        Quaternion quaternion_;
        Vector2 vector2_;
        Vector3 vector3_;
        Vector4 vector4_;
        Vector2i vector2i_;
        Vector3i vector3i_;
        Vector4i vector4i_;
        Matrix2 matrix2_;
        Matrix3 matrix3_;
        Matrix4 matrix4_;
        Matrix2i matrix2i_;
        Matrix3i matrix3i_;
        Matrix4i matrix4i_;
        SharedPtr<RefCounted> ptr_;
        void *rawPtr_;
        Vector vector_;
        Map map_;

        Value() : int_(0)
        {
        }

        Value(const Value &value) = delete;

        ~Value() // NOLINT(modernize-use-equals-default,hicpp-use-equals-default)
        {
        }
    };

public:
    enum Type
    {
        kTypeInt,
        kTypeInt64,
        kTypeFloat,
        kTypeDouble,
        kTypeBool,
        kTypeString,
        kTypeColor,
        kTypeQuaternion,
        kTypeVector2f,
        kTypeVector3f,
        kTypeVector4f,
        kTypeVector2i,
        kTypeVector3i,
        kTypeVector4i,
        kTypeMatrix2f,
        kTypeMatrix3f,
        kTypeMatrix4f,
        kTypeMatrix2i,
        kTypeMatrix3i,
        kTypeMatrix4i,
        kTypePtr,
        kTypeRawPtr,
        kTypeVector,
        kTypeMap,

        kTypeCount
    };

    Variant() : type_(kTypeInt), value_()
    {
    }

    Variant(const Variant &v) : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(Variant &&v) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = std::move(v);
    }

    Variant(int v) : type_(kTypeInt), value_() // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        *this = v;
    }

    Variant(int64_t v) : type_(kTypeInt), value_() // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        *this = v;
    }

    Variant(float v) : type_(kTypeInt), value_() // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        *this = v;
    }

    Variant(double v) : type_(kTypeInt), value_() // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        *this = v;
    }

    Variant(bool v) : type_(kTypeInt), value_() // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        *this = v;
    }

    Variant(const std::string &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const char *v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Color &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Quaternion &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Vector2 &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Vector3 &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Vector4 &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Vector2i &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Vector3i &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Vector4i &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Matrix2 &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Matrix3 &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Matrix4 &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Matrix2i &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Matrix3i &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Matrix4i &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const SharedPtr<RefCounted> &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    template <class T>
    Variant(const SharedPtr<T> &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = SharedPtr<RefCounted>(v);
    }

    Variant(void *v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Vector &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    Variant(const Map &v) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : type_(kTypeInt), value_()
    {
        *this = v;
    }

    ~Variant()
    {
        SetType(kTypeInt);
    }

    Variant &operator=(int v)
    {
        SetType(kTypeInt);
        value_.int_ = v;
        return *this;
    }

    Variant &operator=(int64_t v)
    {
        SetType(kTypeInt64);
        value_.int64_ = v;
        return *this;
    }

    Variant &operator=(float v)
    {
        SetType(kTypeFloat);
        value_.float_ = v;
        return *this;
    }

    Variant &operator=(double v)
    {
        SetType(kTypeDouble);
        value_.double_ = v;
        return *this;
    }

    Variant &operator=(bool v)
    {
        SetType(kTypeBool);
        value_.bool_ = v;
        return *this;
    }

    Variant &operator=(const std::string &v)
    {
        SetType(kTypeString);
        value_.string_ = v;
        return *this;
    }

    Variant &operator=(const char *v)
    {
        *this = std::string(v);
        return *this;
    }

    Variant &operator=(const Color &v)
    {
        SetType(kTypeColor);
        value_.color_ = v;
        return *this;
    }

    Variant &operator=(const Quaternion &v)
    {
        SetType(kTypeQuaternion);
        value_.quaternion_ = v;
        return *this;
    }

    Variant &operator=(const Vector2 &v)
    {
        SetType(kTypeVector2f);
        value_.vector2_ = v;
        return *this;
    }

    Variant &operator=(const Vector3 &v)
    {
        SetType(kTypeVector3f);
        value_.vector3_ = v;
        return *this;
    }

    Variant &operator=(const Vector4 &v)
    {
        SetType(kTypeVector4f);
        value_.vector4_ = v;
        return *this;
    }

    Variant &operator=(const Vector2i &v)
    {
        SetType(kTypeVector2i);
        value_.vector2i_ = v;
        return *this;
    }

    Variant &operator=(const Vector3i &v)
    {
        SetType(kTypeVector3i);
        value_.vector3i_ = v;
        return *this;
    }

    Variant &operator=(const Vector4i &v)
    {
        SetType(kTypeVector4i);
        value_.vector4i_ = v;
        return *this;
    }

    Variant &operator=(const Matrix2 &v)
    {
        SetType(kTypeMatrix2f);
        value_.matrix2_ = v;
        return *this;
    }

    Variant &operator=(const Matrix3 &v)
    {
        SetType(kTypeMatrix3f);
        value_.matrix3_ = v;
        return *this;
    }

    Variant &operator=(const Matrix4 &v)
    {
        SetType(kTypeMatrix4f);
        value_.matrix4_ = v;
        return *this;
    }

    Variant &operator=(const Matrix2i &v)
    {
        SetType(kTypeMatrix2i);
        value_.matrix2i_ = v;
        return *this;
    }

    Variant &operator=(const Matrix3i &v)
    {
        SetType(kTypeMatrix3i);
        value_.matrix3i_ = v;
        return *this;
    }

    Variant &operator=(const Matrix4i &v)
    {
        SetType(kTypeMatrix4i);
        value_.matrix4i_ = v;
        return *this;
    }

    Variant &operator=(const SharedPtr<RefCounted> &v)
    {
        SetType(kTypePtr);
        value_.ptr_ = v;
        return *this;
    }

    Variant &operator=(void *v)
    {
        SetType(kTypeRawPtr);
        value_.rawPtr_ = v;
        return *this;
    }

    Variant &operator=(const Vector &v)
    {
        SetType(kTypeVector);
        value_.vector_ = v;
        return *this;
    }

    Variant &operator=(const Map &v)
    {
        SetType(kTypeMap);
        value_.map_ = v;
        return *this;
    }

    Variant &operator=(const Variant &v);

    Variant &operator=(Variant &&v) noexcept;

    Type GetType() const
    {
        return type_;
    }

    int GetInt() const
    {
        SSASSERT(type_ == kTypeInt);
        return value_.int_;
    }
    int64_t GetInt64() const
    {
        SSASSERT(type_ == kTypeInt64);
        return value_.int64_;
    }
    float GetFloat() const
    {
        SSASSERT(type_ == kTypeFloat);
        return value_.float_;
    }
    double GetDouble() const
    {
        SSASSERT(type_ == kTypeDouble);
        return value_.double_;
    }
    bool GetBool() const
    {
        SSASSERT(type_ == kTypeBool);
        return value_.bool_;
    }
    String GetString() const
    {
        SSASSERT(type_ == kTypeString);
        return value_.string_;
    }
    Color GetColor() const
    {
        SSASSERT(type_ == kTypeColor);
        return value_.color_;
    }
    Quaternion GetQuaternion() const
    {
        SSASSERT(type_ == kTypeQuaternion);
        return value_.quaternion_;
    }
    Vector2 GetVector2f() const
    {
        SSASSERT(type_ == kTypeVector2f);
        return value_.vector2_;
    }
    Vector3 GetVector3f() const
    {
        SSASSERT(type_ == kTypeVector3f);
        return value_.vector3_;
    }
    Vector4 GetVector4f() const
    {
        SSASSERT(type_ == kTypeVector4f);
        return value_.vector4_;
    }
    Vector2i GetVector2i() const
    {
        SSASSERT(type_ == kTypeVector2i);
        return value_.vector2i_;
    }
    Vector3i GetVector3i() const
    {
        SSASSERT(type_ == kTypeVector3i);
        return value_.vector3i_;
    }
    Vector4i GetVector4i() const
    {
        SSASSERT(type_ == kTypeVector4i);
        return value_.vector4i_;
    }
    Matrix2 GetMatrix2f() const
    {
        SSASSERT(type_ == kTypeMatrix2f);
        return value_.matrix2_;
    }
    Matrix3 GetMatrix3f() const
    {
        SSASSERT(type_ == kTypeMatrix3f);
        return value_.matrix3_;
    }
    Matrix4 GetMatrix4f() const
    {
        SSASSERT(type_ == kTypeMatrix4f);
        return value_.matrix4_;
    }
    Matrix2i GetMatrix2i() const
    {
        SSASSERT(type_ == kTypeMatrix2i);
        return value_.matrix2i_;
    }
    Matrix3i GetMatrix3i() const
    {
        SSASSERT(type_ == kTypeMatrix3i);
        return value_.matrix3i_;
    }
    Matrix4i GetMatrix4i() const
    {
        SSASSERT(type_ == kTypeMatrix4i);
        return value_.matrix4i_;
    }
    SharedPtr<RefCounted> GetPtr() const
    {
        SSASSERT(type_ == kTypePtr);
        return value_.ptr_;
    }
    void *GetRawPtr() const
    {
        SSASSERT(type_ == kTypeRawPtr);
        return value_.rawPtr_;
    }
    Vector &GetVector()
    {
        SSASSERT(type_ == kTypeVector);
        return value_.vector_;
    }
    Map &GetMap()
    {
        SSASSERT(type_ == kTypeMap);
        return value_.map_;
    }

    template <class T> T Get();

    template <> int Get()
    {
        return GetInt();
    }
    template <> int64_t Get()
    {
        return GetInt64();
    }
    template <> float Get()
    {
        return GetFloat();
    }
    template <> double Get()
    {
        return GetDouble();
    }
    template <> bool Get()
    {
        return GetBool();
    }
    template <> String Get()
    {
        return GetString();
    }
    template <> Color Get()
    {
        return GetColor();
    }
    template <> Quaternion Get()
    {
        return GetQuaternion();
    }
    template <> Vector2 Get()
    {
        return GetVector2f();
    }
    template <> Vector3 Get()
    {
        return GetVector3f();
    }
    template <> Vector4 Get()
    {
        return GetVector4f();
    }
    template <> Vector2i Get()
    {
        return GetVector2i();
    }
    template <> Vector3i Get()
    {
        return GetVector3i();
    }
    template <> Vector4i Get()
    {
        return GetVector4i();
    }
    template <> Matrix2 Get()
    {
        return GetMatrix2f();
    }
    template <> Matrix3 Get()
    {
        return GetMatrix3f();
    }
    template <> Matrix4 Get()
    {
        return GetMatrix4f();
    }
    template <> Matrix2i Get()
    {
        return GetMatrix2i();
    }
    template <> Matrix3i Get()
    {
        return GetMatrix3i();
    }
    template <> Matrix4i Get()
    {
        return GetMatrix4i();
    }
    template <> SharedPtr<RefCounted> Get()
    {
        return GetPtr();
    }
    template <> void *Get()
    {
        return GetRawPtr();
    }
    template <> Vector &Get()
    {
        return GetVector();
    }
    template <> Map &Get()
    {
        return GetMap();
    }

private:
    void SetType(Type t);

private:
    Type type_;
    Value value_;

    friend bool operator==(const Variant &a, const Variant &b);
    friend bool operator!=(const Variant &a, const Variant &b);
};

bool operator==(const Variant &a, const Variant &b);
bool operator!=(const Variant &a, const Variant &b);

template <class T> static Variant::Type GetVariantType();
template <> static Variant::Type GetVariantType<int64_t>()
{
    return Variant::kTypeInt;
}
template <> static Variant::Type GetVariantType<float>()
{
    return Variant::kTypeInt64;
}
template <> static Variant::Type GetVariantType<double>()
{
    return Variant::kTypeFloat;
}
template <> static Variant::Type GetVariantType<bool>()
{
    return Variant::kTypeDouble;
}
template <> static Variant::Type GetVariantType<Variant::String>()
{
    return Variant::kTypeBool;
}
template <> static Variant::Type GetVariantType<Color>()
{
    return Variant::kTypeString;
}
template <> static Variant::Type GetVariantType<Quaternion>()
{
    return Variant::kTypeColor;
}
template <> static Variant::Type GetVariantType<Vector2>()
{
    return Variant::kTypeQuaternion;
}
template <> static Variant::Type GetVariantType<Vector3>()
{
    return Variant::kTypeVector2f;
}
template <> static Variant::Type GetVariantType<Vector4>()
{
    return Variant::kTypeVector3f;
}
template <> static Variant::Type GetVariantType<Vector2i>()
{
    return Variant::kTypeVector4f;
}
template <> static Variant::Type GetVariantType<Vector3i>()
{
    return Variant::kTypeVector2i;
}
template <> static Variant::Type GetVariantType<Vector4i>()
{
    return Variant::kTypeVector3i;
}
template <> static Variant::Type GetVariantType<Matrix2>()
{
    return Variant::kTypeVector4i;
}
template <> static Variant::Type GetVariantType<Matrix3>()
{
    return Variant::kTypeMatrix2f;
}
template <> static Variant::Type GetVariantType<Matrix4>()
{
    return Variant::kTypeMatrix3f;
}
template <> static Variant::Type GetVariantType<Matrix2i>()
{
    return Variant::kTypeMatrix4f;
}
template <> static Variant::Type GetVariantType<Matrix3i>()
{
    return Variant::kTypeMatrix2i;
}
template <> static Variant::Type GetVariantType<Matrix4i>()
{
    return Variant::kTypeMatrix3i;
}
template <> static Variant::Type GetVariantType<SharedPtr<RefCounted>>()
{
    return Variant::kTypeMatrix4i;
}
template <> static Variant::Type GetVariantType<void *>()
{
    return Variant::kTypePtr;
}
template <> static Variant::Type GetVariantType<Variant::Vector>()
{
    return Variant::kTypeRawPtr;
}
template <> static Variant::Type GetVariantType<Variant::Map>()
{
    return Variant::kTypeVector;
}

} // namespace ss