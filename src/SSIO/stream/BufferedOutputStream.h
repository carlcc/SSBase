//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../../SSBase/Buffer.h"
#include "OutputStream.h"

namespace ss {

class BufferedOutputStream : public OutputStream {
    SS_OBJECT(BufferedOutputStream, OutputStream);

public:
    explicit BufferedOutputStream(OutputStream& stream, uint32_t bufferSize = 4096)
        : stream_(stream)
        , buffer_(bufferSize)
    {
    }
    explicit BufferedOutputStream(OutputStream* stream, uint32_t bufferSize = 4096)
        : stream_(*stream)
        , buffer_(bufferSize)
    {
        SSASSERT(stream != nullptr);
    }
    BufferedOutputStream(const BufferedOutputStream&) = delete;
    BufferedOutputStream(BufferedOutputStream&&) = delete;
    BufferedOutputStream& operator=(const BufferedOutputStream&) = delete;
    BufferedOutputStream& operator=(BufferedOutputStream&&) = delete;

    ~BufferedOutputStream() override
    {
        BufferedOutputStream::Close();
    }

    int Write(uint8_t byte) override;

    int32_t Write(const void* data, uint32_t count) override;

    void Close() override
    {
        if (stream_.IsValid()) {
            Flush();
        }
    }

    bool IsValid() const override
    {
        return stream_.IsValid();
    }

    int32_t Flush() override;

private:
    OutputStream& stream_;
    DynamicBuffer buffer_;
};

} // namespace ss