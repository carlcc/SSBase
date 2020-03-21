//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "InputStream.h"

namespace ss {

class InputStreamReader : public Object {
    SS_OBJECT(InputStreamReader, Object)
public:
    explicit InputStreamReader(InputStream* stream)
        : stream_(*stream)
    {
        SSASSERT(stream != nullptr);
    }
    explicit InputStreamReader(InputStream& stream)
        : stream_(stream)
    {
    }

    uint32_t Read(void* buffer, uint32_t length)
    {
        return stream_.Read(buffer, length);
    }

    int8_t ReadInt8(bool* ok = nullptr) const
    {
        int ret = stream_.Read();
        if (ok != nullptr) {
            *ok = ret < 0;
        }
        return ret;
    }

    uint8_t ReadUint8(bool* ok = nullptr) const
    {
        return ReadInt8(ok);
    }

    int16_t ReadInt16(bool* ok = nullptr) const
    {
        return ReadUint16(ok);
    }
    int16_t ReadInt16BE(bool* ok = nullptr) const
    {
        return ReadUint16BE(ok);
    }
    int16_t ReadInt16LE(bool* ok = nullptr) const
    {
        return ReadUint16LE(ok);
    }

    uint16_t ReadUint16(bool* ok = nullptr) const
    {
        uint16_t data;
        int ret = stream_.Read(&data, 2);
        if (ok != nullptr) {
            *ok = ret == 2;
        }
        return data;
    }
    uint16_t ReadUint16BE(bool* ok = nullptr) const
    {
        uint8_t data[2];
        int ret = stream_.Read(data, 2);
        if (ok != nullptr) {
            *ok = ret == 2;
        }
        return (uint32_t(data[0]) << 8u) | uint32_t(data[1]);
    }
    uint16_t ReadUint16LE(bool* ok = nullptr) const
    {
        uint8_t data[2];
        int ret = stream_.Read(data, 2);
        if (ok != nullptr) {
            *ok = ret == 2;
        }
        return (uint32_t(data[1]) << 8u) | uint32_t(data[0]);
    }

    int32_t ReadInt32(bool* ok = nullptr) const
    {
        return ReadUint32(ok);
    }
    int32_t ReadInt32BE(bool* ok = nullptr) const
    {
        return ReadUint32BE(ok);
    }
    int32_t ReadInt32LE(bool* ok = nullptr) const
    {
        return ReadUint32LE(ok);
    }

    uint32_t ReadUint32(bool* ok = nullptr) const
    {
        uint32_t data;
        int ret = stream_.Read(&data, 4);
        if (ok != nullptr) {
            *ok = ret == 4;
        }
        return data;
    }
    uint32_t ReadUint32BE(bool* ok = nullptr) const
    {
        uint8_t data[4];
        int ret = stream_.Read(data, 4);
        if (ok != nullptr) {
            *ok = ret == 4;
        }
        return (uint32_t(data[0]) << 24u) | (uint32_t(data[1]) << 16u) | ((uint32_t(data[2]) << 8u) | uint32_t(data[3]));
    }
    uint32_t ReadUint32LE(bool* ok = nullptr) const
    {
        uint8_t data[4];
        int ret = stream_.Read(data, 4);
        if (ok != nullptr) {
            *ok = ret == 4;
        }
        return (uint32_t(data[3]) << 24u) | (uint32_t(data[2]) << 16u) | ((uint32_t(data[1]) << 8u) | uint32_t(data[0]));
    }

    int64_t ReadInt64(bool* ok = nullptr) const
    {
        return ReadUint64(ok);
    }
    int64_t ReadInt64BE(bool* ok = nullptr) const
    {
        return ReadUint64BE(ok);
    }
    int64_t ReadInt64LE(bool* ok = nullptr) const
    {
        return ReadUint64LE(ok);
    }

    uint64_t ReadUint64(bool* ok = nullptr) const
    {
        uint64_t data;
        int ret = stream_.Read(&data, 8);
        if (ok != nullptr) {
            *ok = ret == 8;
        }
        return data;
    }
    uint64_t ReadUint64BE(bool* ok = nullptr) const
    {
        uint8_t data[8];
        int ret = stream_.Read(data, 8);
        if (ok != nullptr) {
            *ok = ret == 8;
        }
        return (uint64_t(data[0]) << 56u) | (uint64_t(data[1]) << 48u) | (uint64_t(data[2]) << 40u) | (uint64_t(data[3]) << 32u) | (uint64_t(data[4]) << 24u) | (uint64_t(data[5]) << 16u) | (uint64_t(data[6]) << 8u) | uint64_t(data[7]);
    }
    uint64_t ReadUint64LE(bool* ok = nullptr) const
    {
        uint8_t data[8];
        int ret = stream_.Read(data, 8);
        if (ok != nullptr) {
            *ok = ret == 8;
        }
        return (uint64_t(data[7]) << 56u) | (uint64_t(data[6]) << 48u) | (uint64_t(data[5]) << 40u) | (uint64_t(data[4]) << 32u) | (uint64_t(data[3]) << 24u) | (uint64_t(data[2]) << 16u) | (uint64_t(data[1]) << 8u) | uint64_t(data[0]);
    }

private:
    InputStream& stream_;
};

}