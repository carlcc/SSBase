//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "OutputStream.h"

namespace ss
{

class FileOutputStream : public OutputStream
{
    SS_OBJECT(FileOutputStream, OutputStream);

public:
    explicit FileOutputStream(const std::string &file);

    explicit FileOutputStream(const char *file);

    ~FileOutputStream() override;

    int Write(uint8_t byte) override;

    int32_t Write(void *buf, uint32_t count) override;

    void Close() override;

    bool IsValid() const override;

private:
    FILE *filePtr_;
};

}