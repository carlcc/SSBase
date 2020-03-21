//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "BufferedInputStream.h"
#include "StreamConstant.h"

namespace ss {

int BufferedInputStream::Read()
{
    uint8_t b;
    auto ret = Read(&b, 1);
    if (ret < 0) {
        return ret;
    }
    return b;
}

int32_t BufferedInputStream::Read(void* buf, uint32_t count)
{
    int32_t readCount = 0;
    auto* ubuf = (uint8_t*)buf;
    while (true) {
        // Read data from buffer
        uint32_t n = count > buffer_.Size() ? buffer_.Size() : count;
        buffer_.ReadData(ubuf, n);
        buffer_.Skip(n);
        readCount += n;
        count -= n;
        ubuf += n;

        if (count == 0) {
            return readCount;
        }

        // Fill buffer
        SSASSERT(buffer_.Empty());
        auto ret = stream_.Read(buffer_.GetBufferHead(), buffer_.Capacity());
        if (ret == StreamConstant::ErrorCode::kEof) {
            return readCount > 0 ? readCount : ret;
        }
        if (ret < 0) {
            return ret;
        }
        if (ret == 0) {
            return readCount;
        }
        buffer_.Reset(0, ret);
    }
}

int32_t BufferedInputStream::Available() const
{
    return buffer_.Size() + stream_.Available();
}

void BufferedInputStream::Close()
{
}

} // namespace ss
