//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../../SSBase/Buffer.h"
#include "InputStream.h"

namespace ss {

class BufferedInputStream : public InputStream {
    SS_OBJECT(BufferedInputStream, InputStream)
public:
    explicit BufferedInputStream(InputStream& stream, uint32_t bufferSize = 4096)
        : stream_(stream)
        , buffer_(bufferSize)
    {
    }
    explicit BufferedInputStream(InputStream* stream, uint32_t bufferSize = 4096)
        : stream_(*stream)
        , buffer_(bufferSize)
    {
        SSASSERT(stream != nullptr);
    }
    BufferedInputStream(const BufferedInputStream&) = delete;
    BufferedInputStream(BufferedInputStream&&) = delete;
    BufferedInputStream& operator=(const BufferedInputStream&) = delete;
    BufferedInputStream& operator=(BufferedInputStream&&) = delete;

    ~BufferedInputStream() override = default;

    int Read() override;

    int32_t Read(void* buf, uint32_t count) override;

    int32_t Available() const override;

    void Close() override;

    bool IsValid() const override
    {
        return stream_.IsValid();
    }

private:
    InputStream& stream_;
    DynamicBuffer buffer_;
};

}