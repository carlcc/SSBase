//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "BufferedOutputStream.h"
#include "StreamConstant.h"

namespace ss {

int BufferedOutputStream::Write(uint8_t byte)
{
    auto ret = Write(&byte, 1);
    if (ret < 0) {
        return 0;
    }
    if (ret == 1) {
        return byte;
    }
    return StreamConstant::ErrorCode::kUnknown;
}

int32_t BufferedOutputStream::Write(const void* data, uint32_t count)
{
    auto* udata = (uint8_t*)data;
    uint32_t freeSpace = buffer_.Capacity() - buffer_.Size();
    if (freeSpace > count) {
        // buffer is big enough, just push data to buffer
        buffer_.PushData(udata, count);
        return count;
    }

    if (count < buffer_.Capacity() + freeSpace) {
        // The buffered bytes and new bytes cannot full fill 2 buffer,
        // then we write the buffer-size bytes, and keep the reset in buffer
        buffer_.PushData(udata, freeSpace);
        SSASSERT(buffer_.Capacity() == buffer_.Size());

        int32_t ret = stream_.Write(buffer_.GetData<void>(), buffer_.Size());
        if (ret < 0) {
            return ret;
        }

        if (uint32_t(ret) < buffer_.Size()) {
            buffer_.Skip(ret);
            // Keep it simple, if not all the data were written, just return
            return freeSpace;
        }

        // all data in out buffer were written
        buffer_.Reset(0, 0);
        buffer_.PushData(udata + freeSpace, count - freeSpace);
        return count;
    }

    // There are no less than 2 buffer-size bytes we need to write.
    // first, we flush the buffered data, and then write the new coming data directly.
    int32_t ret = stream_.Write(buffer_.GetData<void>(), buffer_.Size());
    if (ret < 0) {
        return ret;
    }
    if (uint32_t(ret) < buffer_.Size()) {
        buffer_.Skip(ret);
        // There are at least `freeSpace` bytes spare space in out buffer
        buffer_.PushData(data, freeSpace);
        return freeSpace;
    }
    ret = stream_.Write(data, count);
    if (ret < 0) {
        return ret;
    }
    ret += buffer_.Size();
    buffer_.Reset();
    return ret;
}

int32_t BufferedOutputStream::Flush()
{
    int32_t ret = stream_.Write(buffer_.GetData<void>(), buffer_.Size());
    if (ret < 0) {
        return ret;
    }
    if (uint32_t(ret) < buffer_.Size()) {
        buffer_.Skip(ret);
        // Not all data were flushed
        return StreamConstant::ErrorCode::kUnknown;
    }
    buffer_.Reset();
    return 0;
}

} // namespace ss
