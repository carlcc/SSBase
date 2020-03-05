//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../../SSBase/Object.h"

namespace ss
{

class OutputStream : public Object
{
    SS_OBJECT(OutputStream, Object);

public:
    OutputStream() = default;
    OutputStream(const OutputStream &) = delete;
    OutputStream(OutputStream &&) = delete;
    OutputStream &operator=(const OutputStream &) = delete;
    OutputStream &operator=(OutputStream &&) = delete;
    ~OutputStream() override = default;

    /// Write 1 byte, returns the byte if succeed, else returns error code on error
    virtual int Write(uint8_t byte) = 0;

    /// Write `count` bytes, returns the actual written bytes number, returns error code on error
    /// NOTE: Invoke Write() function by default, you'd better override it for better performance.
    virtual int32_t Write(const void *data, uint32_t count);

    /// NOTE: This function is provide here just for early closing a file. The destructor `~InputStream()`
    /// is not able to invoke the overrides of `Close` function, so you may need to release resources
    /// in subclasses' destructors.
    virtual void Close() = 0;

    /// Return true if this stream is valid, else false
    virtual bool IsValid() const = 0;

    /// Flush data
    virtual int32_t Flush() = 0;

    /// Returns true if this stream is not valid, else false
    bool operator!() const
    {
        return !IsValid();
    }

    /// Return true if this stream is valid, else false
    explicit operator bool() const
    {
        return IsValid();
    }
};

} // namespace ss