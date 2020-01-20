//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "InputStream.h"
#include <cstdio>

namespace ss
{

class FileInputStream : public InputStream
{
    SS_OBJECT(FileInputStream, InputStream);

public:
    explicit FileInputStream(const std::string &file);

    explicit FileInputStream(const char *file);

    ~FileInputStream() override;

    int Read() override;

    int32_t Read(void *buf, uint32_t count) override;

    int64_t Skip(int64_t n) override;

    int32_t Available() const override;

    void Close() override;

    bool IsValid() const override;

private:
    FILE *filePtr_;
    int64_t totalFileSize_;
};

}