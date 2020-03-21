//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Variant.h"
#include "Ptr.h"

namespace ss {

void Variant::SetType(Variant::Type t)
{
    if (type_ == t) {
        return;
    }
    switch (type_) {
    case Type::kTypeInt:
    case Type::kTypeInt64:
    case Type::kTypeFloat:
    case Type::kTypeDouble:
    case Type::kTypeBool:
    case Type::kTypeColor:
    case Type::kTypeQuaternion:
    case Type::kTypeVector2f:
    case Type::kTypeVector3f:
    case Type::kTypeVector4f:
    case Type::kTypeVector2i:
    case Type::kTypeVector3i:
    case Type::kTypeVector4i:
    case Type::kTypeMatrix2f:
    case Type::kTypeMatrix3f:
    case Type::kTypeMatrix4f:
    case Type::kTypeMatrix2i:
    case Type::kTypeMatrix3i:
    case Type::kTypeMatrix4i:
    case Type::kTypeRawPtr:
        break;
    case Type::kTypeString:
        value_.string_.~String();
        break;
    case Type::kTypePtr:
        value_.ptr_.~SharedPtr<RefCounted>();
        break;
    case Type::kTypeVector:
        value_.vector_.~Vector();
        break;
    case Type::kTypeMap:
        value_.map_.~Map();
        break;
    default:
        SSASSERT(false);
    }

    type_ = t;

    switch (type_) {
    case Type::kTypeInt:
    case Type::kTypeInt64:
    case Type::kTypeFloat:
    case Type::kTypeDouble:
    case Type::kTypeBool:
    case Type::kTypeColor:
    case Type::kTypeQuaternion:
    case Type::kTypeVector2f:
    case Type::kTypeVector3f:
    case Type::kTypeVector4f:
    case Type::kTypeVector2i:
    case Type::kTypeVector3i:
    case Type::kTypeVector4i:
    case Type::kTypeMatrix2f:
    case Type::kTypeMatrix3f:
    case Type::kTypeMatrix4f:
    case Type::kTypeMatrix2i:
    case Type::kTypeMatrix3i:
    case Type::kTypeMatrix4i:
    case Type::kTypeRawPtr:
        break;
    case Type::kTypeString:
        new (&value_.string_) String;
        break;
    case Type::kTypePtr:
        new (&value_.ptr_) SharedPtr<RefCounted>;
        break;
    case Type::kTypeVector:
        new (&value_.vector_) Vector;
        break;
    case Type::kTypeMap:
        new (&value_.map_) Map;
        break;
    default:
        SSASSERT(false);
    }
}

Variant& Variant::operator=(const Variant& v)
{
    SetType(v.type_);

    switch (type_) {
    case Type::kTypeInt:
        value_.int_ = v.value_.int_;
        break;
    case Type::kTypeInt64:
        value_.int64_ = v.value_.int64_;
        break;
    case Type::kTypeFloat:
        value_.float_ = v.value_.float_;
        break;
    case Type::kTypeDouble:
        value_.double_ = v.value_.double_;
        break;
    case Type::kTypeBool:
        value_.bool_ = v.value_.bool_;
        break;
    case Type::kTypeString:
        value_.string_ = v.value_.string_;
        break;
    case Type::kTypeColor:
        value_.color_ = v.value_.color_;
        break;
    case Type::kTypeQuaternion:
        value_.quaternion_ = v.value_.quaternion_;
        break;
    case Type::kTypeVector2f:
        value_.vector2_ = v.value_.vector2_;
        break;
    case Type::kTypeVector3f:
        value_.vector3_ = v.value_.vector3_;
        break;
    case Type::kTypeVector4f:
        value_.vector4_ = v.value_.vector4_;
        break;
    case Type::kTypeVector2i:
        value_.vector2i_ = v.value_.vector2i_;
        break;
    case Type::kTypeVector3i:
        value_.vector3i_ = v.value_.vector3i_;
        break;
    case Type::kTypeVector4i:
        value_.vector4i_ = v.value_.vector4i_;
        break;
    case Type::kTypeMatrix2f:
        value_.matrix2_ = v.value_.matrix2_;
        break;
    case Type::kTypeMatrix3f:
        value_.matrix3_ = v.value_.matrix3_;
        break;
    case Type::kTypeMatrix4f:
        value_.matrix4_ = v.value_.matrix4_;
        break;
    case Type::kTypeMatrix2i:
        value_.matrix2i_ = v.value_.matrix2i_;
        break;
    case Type::kTypeMatrix3i:
        value_.matrix3i_ = v.value_.matrix3i_;
        break;
    case Type::kTypeMatrix4i:
        value_.matrix4i_ = v.value_.matrix4i_;
        break;
    case Type::kTypeRawPtr:
        value_.rawPtr_ = v.value_.rawPtr_;
        break;
    case Type::kTypePtr:
        value_.ptr_ = v.value_.ptr_;
        break;
    case Type::kTypeVector:
        value_.vector_ = v.value_.vector_;
        break;
    case Type::kTypeMap:
        value_.map_ = v.value_.map_;
        break;
    default:
        break;
    }

    return *this;
}

Variant& Variant::operator=(Variant&& v) noexcept
{
    SetType(v.type_);

    switch (type_) {
    case Type::kTypeInt:
        value_.int_ = v.value_.int_;
        break;
    case Type::kTypeInt64:
        value_.int64_ = v.value_.int64_;
        break;
    case Type::kTypeFloat:
        value_.float_ = v.value_.float_;
        break;
    case Type::kTypeDouble:
        value_.double_ = v.value_.double_;
        break;
    case Type::kTypeBool:
        value_.bool_ = v.value_.bool_;
        break;
    case Type::kTypeString:
        value_.string_ = std::move(v.value_.string_);
        break;
    case Type::kTypeColor:
        value_.color_ = v.value_.color_;
        break;
    case Type::kTypeQuaternion:
        value_.quaternion_ = v.value_.quaternion_;
        break;
    case Type::kTypeVector2f:
        value_.vector2_ = v.value_.vector2_;
        break;
    case Type::kTypeVector3f:
        value_.vector3_ = v.value_.vector3_;
        break;
    case Type::kTypeVector4f:
        value_.vector4_ = v.value_.vector4_;
        break;
    case Type::kTypeVector2i:
        value_.vector2i_ = v.value_.vector2i_;
        break;
    case Type::kTypeVector3i:
        value_.vector3i_ = v.value_.vector3i_;
        break;
    case Type::kTypeVector4i:
        value_.vector4i_ = v.value_.vector4i_;
        break;
    case Type::kTypeMatrix2f:
        value_.matrix2_ = v.value_.matrix2_;
        break;
    case Type::kTypeMatrix3f:
        value_.matrix3_ = v.value_.matrix3_;
        break;
    case Type::kTypeMatrix4f:
        value_.matrix4_ = v.value_.matrix4_;
        break;
    case Type::kTypeMatrix2i:
        value_.matrix2i_ = v.value_.matrix2i_;
        break;
    case Type::kTypeMatrix3i:
        value_.matrix3i_ = v.value_.matrix3i_;
        break;
    case Type::kTypeMatrix4i:
        value_.matrix4i_ = v.value_.matrix4i_;
        break;
    case Type::kTypeRawPtr:
        value_.rawPtr_ = v.value_.rawPtr_;
        break;
    case Type::kTypePtr:
        value_.ptr_ = std::move(v.value_.ptr_);
        break;
    case Type::kTypeVector:
        value_.vector_ = std::move(v.value_.vector_);
        break;
    case Type::kTypeMap:
        value_.map_ = std::move(v.value_.map_);
        break;
    default:
        break;
    }

    v = int(0);
    return *this;
}

bool operator==(const Variant& a, const Variant& b)
{
    if (a.GetType() != b.GetType()) {
        return false;
    }

    using Type = Variant::Type;
    switch (a.GetType()) {
    case Type::kTypeInt:
        return a.value_.int_ == b.value_.int_;
    case Type::kTypeInt64:
        return a.value_.int64_ == b.value_.int64_;
    case Type::kTypeFloat:
        return a.value_.float_ == b.value_.float_;
    case Type::kTypeDouble:
        return a.value_.double_ == b.value_.double_;
    case Type::kTypeBool:
        return a.value_.bool_ == b.value_.bool_;
    case Type::kTypeString:
        return a.value_.string_ == b.value_.string_;
    case Type::kTypeColor:
        return a.value_.color_ == b.value_.color_;
    case Type::kTypeQuaternion:
        return a.value_.quaternion_ == b.value_.quaternion_;
    case Type::kTypeVector2f:
        return a.value_.vector2_ == b.value_.vector2_;
    case Type::kTypeVector3f:
        return a.value_.vector3_ == b.value_.vector3_;
    case Type::kTypeVector4f:
        return a.value_.vector4_ == b.value_.vector4_;
    case Type::kTypeVector2i:
        return a.value_.vector2i_ == b.value_.vector2i_;
    case Type::kTypeVector3i:
        return a.value_.vector3i_ == b.value_.vector3i_;
    case Type::kTypeVector4i:
        return a.value_.vector4i_ == b.value_.vector4i_;
    case Type::kTypeMatrix2f:
        return a.value_.matrix2_ == b.value_.matrix2_;
    case Type::kTypeMatrix3f:
        return a.value_.matrix3_ == b.value_.matrix3_;
    case Type::kTypeMatrix4f:
        return a.value_.matrix4_ == b.value_.matrix4_;
    case Type::kTypeMatrix2i:
        return a.value_.matrix2i_ == b.value_.matrix2i_;
    case Type::kTypeMatrix3i:
        return a.value_.matrix3i_ == b.value_.matrix3i_;
    case Type::kTypeMatrix4i:
        return a.value_.matrix4i_ == b.value_.matrix4i_;
    case Type::kTypePtr:
        return a.value_.ptr_ == b.value_.ptr_;
    case Type::kTypeRawPtr:
        return a.value_.rawPtr_ == b.value_.rawPtr_;
    case Type::kTypeVector:
        return a.value_.vector_ == b.value_.vector_;
    case Type::kTypeMap:
        return a.value_.map_ == b.value_.map_;
    default:
        SSASSERT(false);
        break;
    }
}

bool operator!=(const Variant& a, const Variant& b)
{
    return !(a == b);
}

} // namespace ss
