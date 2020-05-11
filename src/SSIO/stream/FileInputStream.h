//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "SeekableInputStream.h"
#include "StreamConstant.h"

#include <cstdio>

namespace ss {

class FileInputStream : public SeekableInputStream {
    SS_OBJECT(FileInputStream, SeekableInputStream);

public:
    explicit FileInputStream(const CharSequence& file);

    explicit FileInputStream(const String& file);

    ~FileInputStream() override;

    int Read() override;

    int32_t Read(void* buf, uint32_t count) override;

    int64_t Skip(int64_t n) override;

    int32_t Available() const override;

    void Close() override;

    bool IsValid() const override;

    std::string ReadAll();

    int Seek(int64_t offset, Whence whence) override;

private:
    void Init(const CharSequence& file);

private:
    FILE* filePtr_;
    int64_t totalFileSize_;
};

} // namespace ss