//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Assert.h"
#include <cstdint>
#include <cstring>

namespace ss
{

template <uint32_t capacity> class Buffer
{
public:
    void Reset(uint32_t offset = 0, uint32_t size = 0)
    {
        offset_ = offset;
        size_ = size;
    }

    const void *GetBufferHead() const
    {
        return buf_;
    }

    void *GetBufferHead()
    {
        return buf_;
    }

    template <class T> T *GetData()
    {
        return reinterpret_cast<T *>(buf_ + offset_);
    }

    template <class T> const T *GetData() const
    {
        return reinterpret_cast<const T *>(buf_ + offset_);
    }

    uint32_t ReadData(void *buffer, uint32_t size) const
    {
        if (size > Size())
        {
            size = Size();
        }
        memcpy(buffer, GetData<void>(), size);
        return size;
    }

    void PushData(const void *data, uint32_t length)
    {
        SSASERT(length <= Capacity() - Size());
        if (offset_ + length <= Capacity())
        {
            memmove(GetBufferHead(), GetData<uint8_t>(), Size());
            offset_ = 0;
        }
        memcpy(GetData<uint8_t>() + Size(), data, length);
        size_ += length;
    }

    uint32_t TryPushData(const void *data, uint32_t length)
    {
        if (length + Size() > Capacity())
        {
            length = Capacity() - Size();
        }
        PushData(data, length);
        return length;
    }

    void Skip(uint32_t n)
    {
        SSASSERT(Size() >= n);
        offset_ += n;
        size_ -= n;
    }

    uint32_t Capacity() const
    {
        return capacity;
    }

    uint32_t Size() const
    {
        return size_;
    }

    bool Empty() const
    {
        return Size() == 0;
    }

    uint8_t operator[](int32_t index) const
    {
        SSASSERT(index >= 0 && index < Size());
        return GetData<uint8_t>()[index];
    }

    uint8_t &operator[](int32_t index)
    {
        SSASSERT(index >= 0 && index < Size());
        return GetData<uint8_t>()[index];
    }

private:
    uint32_t offset_;
    uint32_t size_;
    uint8_t buf_[capacity];
};

}