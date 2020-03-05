//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "InputStream.h"

namespace ss
{

class Lz4InputStream : public InputStream
{
    SS_OBJECT(Lz4InputStream, InputStream);

public:
    explicit Lz4InputStream(InputStream *stream);
    ~Lz4InputStream() override;
    int Read() override;
    int32_t Read(void *buf, uint32_t count) override;
    int64_t Skip(int64_t n) override;
    int32_t Available() const override;
    void Close() override;
    bool IsValid() const override;

private:
    class Impl;
    Impl *impl_;
};

}