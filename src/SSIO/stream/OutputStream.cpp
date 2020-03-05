//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "OutputStream.h"
#include "StreamConstant.h"

namespace ss
{

int32_t OutputStream::Write(const void *buf, uint32_t count)
{
    int32_t c = 0;
    auto *pBuf = static_cast<const uint8_t *>(buf);

    for (uint32_t i = 0; i < count; ++i)
    {
        int ch = Write(pBuf[i]);
        if (0 < ch && ch < 256)
        {
            return ch;
        }
        ++c;
    }
    return c;
}

} // namespace ss
