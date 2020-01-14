//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Variant.h"

namespace ss
{

void Variant::SetType(Variant::Type t)
{
    if (type_ == t)
    {
        return;
    }
    switch (type_)
    {
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
    default:
        SSASSERT(false);
        break;
    case Type::kTypeString:
        value_.string_.~String();
    case Type::kTypePtr:
        value_.ptr_.~SharedPtr<RefCounted>();
        break;
    case Type::kTypeVector:
        value_.vector_.~Vector();
        break;
    case Type::kTypeMap:
        value_.map_.~Map();
        break;
    }

    type_ = t;

    switch (type_)
    {
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
    default:
        SSASSERT(false);
        break;
    case Type::kTypeString:
        new (&value_.string_) String;
    case Type::kTypePtr:
        new (&value_.ptr_) SharedPtr<RefCounted>;
        break;
    case Type::kTypeVector:
        new (&value_.vector_) Vector;
        break;
    case Type::kTypeMap:
        new (&value_.map_) Map;
        break;
    }
}
Variant &Variant::operator=(const Variant &v)
{
    SetType(v.type_);

    switch (type_)
    {
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

} // namespace ss
