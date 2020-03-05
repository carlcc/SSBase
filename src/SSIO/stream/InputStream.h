//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../../SSBase/Object.h"
#include <cstdint>

namespace ss
{

class InputStream : public Object
{
    SS_OBJECT(InputStream, Object);

public:
    InputStream() = default;
    InputStream(const InputStream &) = delete;
    InputStream(InputStream &&) = delete;
    InputStream &operator=(const InputStream &) = delete;
    InputStream &operator=(InputStream &&) = delete;
    ~InputStream() override = default;

    /// Read 1 byte, returns the byte, returns -1 on EOF reached, else return the error code
    virtual int Read() = 0;

    /// Read at most `count` bytes, returns the actual read bytes number, returns -1 if EOF
    /// Reached, else return the error code.
    /// NOTE: Invoke Read() function by default, you'd better override it for better performance.
    virtual int32_t Read(void *buf, uint32_t count);

    /// Skip 'n' bytes from stream, returns the actual skipped bytes number.
    /// NOTE: Invoke Read(void*, uint32_t) byte default, you may need to override it for better performance.
    virtual int64_t Skip(int64_t n);

    /// Returns the currently available bytes number. The 'available' means we can read it
    /// immediately without blocking.
    virtual int32_t Available() const = 0;

    /// NOTE: This function is provide here just for early closing a file. The destructor `~InputStream()`
    /// is not able to invoke the overrides of `Close` function, so you may need to release resources
    /// in subclasses' destructors.
    virtual void Close() = 0;

    /// Return true if this stream is valid, else false
    virtual bool IsValid() const = 0;

    /// Returns true if this stream is not valid, else false
    bool operator!() const
    {
        return !IsValid();
    }

    /// Return true if this stream is valid, else false
    explicit operator bool()
    {
        return IsValid();
    }
};

} // namespace ss