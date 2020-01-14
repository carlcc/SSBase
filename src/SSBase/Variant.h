//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Color.h"
#include "LinearMath.h"
#include "Ptr.h"
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace ss
{

class Variant
{
    using String = std::string;
    using Map = std::map<std::string, Variant>;
    using Vector = std::vector<Variant>;
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

    Type GetType() const
    {
        return type_;
    }

    void SetType(Type t);

private:
    Type type_;
    Value value_;
};

} // namespace ss