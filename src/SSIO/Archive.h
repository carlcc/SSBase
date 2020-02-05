//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../SSBase/Object.h"
#include "../SSBase/Ptr.h"

namespace ss
{

class InputStream;

class Archive : public Object
{
    SS_OBJECT(Archive, Object);

public:
    class Entry {

    };
    Archive(const Archive &) = delete;
    Archive(Archive &&) = delete;
    Archive &operator=(const Archive &) = delete;
    Archive &operator=(Archive &&) = delete;


};

}