//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../SSBase/Object.h"
#include "../SSBase/Ptr.h"
#include <map>

namespace ss {

class InputStream;

class Archive : public Object {
    SS_OBJECT(Archive, Object);

public:
    using EntryList = std::vector<String>;

    explicit Archive(const CharSequence& filePath);
    explicit Archive(const String& filePath);
    Archive(const Archive&) = delete;
    Archive(Archive&&) = delete;
    Archive& operator=(const Archive&) = delete;
    Archive& operator=(Archive&&) = delete;
    ~Archive() override;

    const EntryList& GetEntries() const;
    SharedPtr<InputStream> GetInputStream(const CharSequence& filePath);
    SharedPtr<InputStream> GetInputStream(const String& filePath);

    bool IsValid() const;
    explicit operator bool() const
    {
        return IsValid();
    }
    bool operator!() const
    {
        return !IsValid();
    }

private:
    class ArchiveInputStream;
    class Impl;
    Impl* impl_;
};

}