//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "OutputStream.h"

namespace ss {

class FileOutputStream : public OutputStream {
    SS_OBJECT(FileOutputStream, OutputStream);

public:
    explicit FileOutputStream(const CharSequence& file);

    explicit FileOutputStream(const String& file);

    ~FileOutputStream() override;

    int Write(uint8_t byte) override;

    int32_t Write(const void* data, uint32_t count) override;

    void Close() override;

    bool IsValid() const override;
    int32_t Flush() override;

private:
    void Init(const CharSequence& file);

private:
    FILE* filePtr_;
};

}