//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "InputStream.h"
#include "StreamConstant.h"

namespace ss
{

int32_t InputStream::Read(void *buf, uint32_t count)
{
    int32_t c = 0;
    auto *pByte = static_cast<uint8_t *>(buf);
    for (uint32_t i = 0; i < count; ++i)
    {
        int ch = Read();
        if (ch < 256)
        {
            pByte[c++] = uint8_t(ch);
        }
        else if (ch == -1) // read EOF
        {
            return c > 0 ? c : -1;
        }
        else
        {
            return ch;
        }
    }
    return c;
}

int64_t InputStream::Skip(int64_t n)
{
    if (n <= 0)
    {
        return 0;
    }
    const int kBufferSize = 4096;
    uint8_t buffer[kBufferSize];
    int64_t leftSize = n;

    int32_t count = Read(buffer, kBufferSize);
    if (count == -1) // EOF
    {
        return 0;
    }
    leftSize -= count;

    while (leftSize > 0)
    {
        count = Read(buffer, kBufferSize);
        if (count > 0)
        {
            leftSize -= count;
        }
        else if (count == StreamConstant::ErrorCode::kEof) // EOF
        {
            break;
        }
        else if (count == 0) // Not available now
        {
            break;
        }
        else // error occurs
        {
            return count; // Error code
        }
    }

    return n - leftSize;
}

} // namespace ss
