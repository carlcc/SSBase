//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "OutputStream.h"

namespace ss
{

class OutputStreamWriter : public Object
{
    SS_OBJECT(OutputStreamWriter, Object);

public:
    explicit OutputStreamWriter(OutputStream *stream) : stream_(*stream)
    {
        SSASSERT(stream != nullptr);
    }
    explicit OutputStreamWriter(OutputStream &stream) : stream_(stream)
    {
    }

    int32_t Write(void *buf, uint32_t count)
    {
        return stream_.Write(buf, count);
    }

    /// Returns how many bytes written
    int32_t WriteUint8(uint8_t b)
    {
        return stream_.Write(&b, 1);
    }
    int32_t WriteInt8(int8_t b)
    {
        return WriteUint8(b);
    }

    int32_t WriteUint16(uint16_t s)
    {
        return stream_.Write(&s, 2);
    }
    int32_t WriteUint16BE(uint16_t s)
    {
        uint8_t data[2];
        data[0] = s >> 8u;
        data[1] = s;
        return stream_.Write(data, 2);
    }
    int32_t WriteUint16LE(uint16_t s)
    {
        uint8_t data[2];
        data[1] = s >> 8u;
        data[0] = s;
        return stream_.Write(data, 2);
    }
    int32_t WriteInt16(int16_t s)
    {
        return WriteUint16(s);
    }
    int32_t WriteInt16BE(int16_t s)
    {
        return WriteUint16BE(s);
    }
    int32_t WriteInt16LE(int16_t s)
    {
        return WriteUint16LE(s);
    }

    int32_t WriteUint32(uint32_t s)
    {
        return stream_.Write(&s, 4);
    }
    int32_t WriteUint32BE(uint32_t s)
    {
        uint8_t data[4];
        data[0] = s >> 24u;
        data[1] = s >> 16u;
        data[2] = s >> 8u;
        data[3] = s;
        return stream_.Write(data, 4);
    }
    int32_t WriteUint32LE(uint32_t s)
    {
        uint8_t data[4];
        data[3] = s >> 24u;
        data[2] = s >> 16u;
        data[1] = s >> 8u;
        data[0] = s;
        return stream_.Write(data, 4);
    }
    int32_t WriteInt32(int32_t s)
    {
        return WriteUint32(s);
    }
    int32_t WriteInt32BE(int32_t s)
    {
        return WriteUint32BE(s);
    }
    int32_t WriteInt32LE(int32_t s)
    {
        return WriteUint32LE(s);
    }

    int32_t WriteUint64(uint64_t s)
    {
        return stream_.Write(&s, 8);
    }
    int32_t WriteUint64BE(uint64_t s)
    {
        uint8_t data[8];
        data[0] = s >> 56u;
        data[1] = s >> 48u;
        data[2] = s >> 40u;
        data[3] = s >> 32u;
        data[4] = s >> 24u;
        data[5] = s >> 16u;
        data[6] = s >> 8u;
        data[7] = s;
        return stream_.Write(data, 8);
    }
    int32_t WriteUint64LE(uint64_t s)
    {
        uint8_t data[8];
        data[7] = s >> 56u;
        data[6] = s >> 48u;
        data[5] = s >> 40u;
        data[4] = s >> 32u;
        data[3] = s >> 24u;
        data[2] = s >> 16u;
        data[1] = s >> 8u;
        data[0] = s;
        return stream_.Write(data, 8);
    }
    int32_t WriteInt64(int64_t s)
    {
        return WriteUint64(s);
    }
    int32_t WriteInt64BE(int64_t s)
    {
        return WriteUint64BE(s);
    }
    int32_t WriteInt64LE(int64_t s)
    {
        return WriteUint64LE(s);
    }

private:
    OutputStream &stream_;
};

}