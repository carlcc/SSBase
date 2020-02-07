//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Buffer.h"
#include "../SSBase/Misc.h"

namespace ss
{

DynamicBuffer::DynamicBuffer(uint32_t capacity) : offset_(0), size_(0), capacity_(0), buf_(nullptr)
{
    if (capacity > 0)
    {
        ReAllocate(Misc::CeilToPowerOfTwo(capacity));
    }
}

DynamicBuffer::~DynamicBuffer()
{
    offset_ = 0;
    size_ = 0;
    capacity_ = 0;
    if (buf_ != nullptr)
    {
        free(buf_);
        buf_ = nullptr;
    }
}
uint32_t DynamicBuffer::ReadData(void *buffer, uint32_t size) const
{
    if (size > Size())
    {
        size = Size();
    }
    memcpy(buffer, GetData<void>(), size);
    return size;
}

void DynamicBuffer::PushData(const void *data, uint32_t length)
{
    EnsureSpace(length);
    memcpy(GetData<uint8_t>() + Size(), data, length);
    size_ += length;
}

void DynamicBuffer::EnsureSpace(uint32_t size)
{
    if (Size() + size > Capacity()) // If we need larger buffer
    {
        uint32_t newCapacity = Capacity() * 2;
        if (newCapacity < Size() + size)
        {
            newCapacity = Misc::CeilToPowerOfTwo(size + Size());
        }
        ReAllocate(newCapacity);
    }

    uint32_t freeSpace = Capacity() - offset_ - Size();
    if (freeSpace >= size) // If we don't need to move data to make more space
    {
        return;
    }

    // make more space by moving data
    memmove(GetBufferHead(), GetData<uint8_t>(), Size());
    offset_ = 0;
}

void DynamicBuffer::ReAllocate(uint32_t length)
{
    if (buf_ == nullptr)
    {
        buf_ = static_cast<uint8_t *>(malloc(length));
    }
    else
    {
        buf_ = static_cast<uint8_t *>(realloc(buf_, length));
    }
    capacity_ = length;
}

} // namespace ss
