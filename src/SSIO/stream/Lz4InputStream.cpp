//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Lz4InputStream.h"
#include "../../SSBase/Buffer.h"
#include "../thirdparty/lz4/lz4.h"
#include "StreamConstant.h"

namespace ss {

const int BLOCK_BYTES = 1024 * 8;

class Lz4InputStream::Impl : public InputStream {
    SS_OBJECT(Lz4InputStream::Impl, InputStream);

public:
    explicit Impl(InputStream* is)
        : inputStream_(is)
        , lz4Stream_(nullptr)
        , lz4StreamDecode_()
        , cmpBuf_()
        , decBuf_()
        , decBufIndex_(0)
    {
        lz4Stream_ = LZ4_createStream();
        LZ4_setStreamDecode(&lz4StreamDecode_, nullptr, 0);
    }

    ~Impl() override
    {
        if (lz4Stream_ != nullptr) {
            LZ4_freeStream(lz4Stream_);
            lz4Stream_ = nullptr;
        }
    }

    int Read() override
    {
        return 0;
    }

    int32_t Read(void* buf, uint32_t count) override
    {
        auto& decBuf = decBuf_[decBufIndex_];
        int32_t readCount = 0;
        if (decBuf.Size() > 0) {
            auto c = decBuf.ReadData(buf, count);
            decBuf.Skip(c);
            count -= c;
            readCount += c;
            buf = (uint8_t*)buf + c;
        }
        while (count != 0) {
            int32_t cmpBytes = 0;
            {
                const size_t readCount0 = readInt(&cmpBytes);
                if (readCount0 != 1 || cmpBytes <= 0) {
                    return StreamConstant::ErrorCode::kUnknown;
                }

                const size_t readCount1 = inputStream_->Read(cmpBuf_.GetBufferHead(), (size_t)cmpBytes);
                if (readCount1 != (size_t)cmpBytes) {
                    return StreamConstant::ErrorCode::kUnknown;
                }
            }

            {
                const int decBytes = LZ4_decompress_safe_continue(&lz4StreamDecode_, (const char*)cmpBuf_.GetBufferHead(),
                    (char*)decBuf.GetBufferHead(), cmpBytes, BLOCK_BYTES);
                decBuf.Reset(0, decBytes);
                if (decBytes <= 0) {
                    return StreamConstant::ErrorCode::kUnknown;
                }
            }

            if (count >= decBuf.Size()) {
                auto c = decBuf.ReadData(buf, decBuf.Size());
                decBuf.Reset();
                count -= c;
                readCount += c;
                buf = (uint8_t*)buf + c;
            } else {
                auto c = decBuf.ReadData(buf, count);
                SSASSERT(c == count);
                decBuf.Skip(c);
                count -= c; // now count == 0
                readCount += c;
                buf = (uint8_t*)buf + c;
            }
        }

        return readCount;
    }

    int64_t Skip(int64_t n) override
    {
        return InputStream::Skip(n);
    }

    int32_t Available() const override
    {
        return 0;
    }

    void Close() override
    {
    }

    bool IsValid() const override
    {
        return lz4Stream_ != nullptr;
    }

private:
    size_t readInt(int32_t* i)
    {
        return inputStream_->Read(i, sizeof(*i));
    }

private:
    InputStream* inputStream_;
    LZ4_stream_t* lz4Stream_;
    LZ4_streamDecode_t lz4StreamDecode_;
    Buffer<LZ4_COMPRESSBOUND(BLOCK_BYTES)> cmpBuf_;
    Buffer<BLOCK_BYTES> decBuf_[2];
    int decBufIndex_;
};

Lz4InputStream::Lz4InputStream(InputStream* stream)
    : impl_(new Impl(stream))
{
}

Lz4InputStream::~Lz4InputStream()
{
    delete impl_;
}

int Lz4InputStream::Read()
{
    return impl_->Read();
}

int32_t Lz4InputStream::Read(void* buf, uint32_t count)
{
    return impl_->Read(buf, count);
}

int64_t Lz4InputStream::Skip(int64_t n)
{
    return impl_->Skip(n);
}

int32_t Lz4InputStream::Available() const
{
    return impl_->Available();
}

void Lz4InputStream::Close()
{
    return impl_->Close();
}

bool Lz4InputStream::IsValid() const
{
    return impl_->IsValid();
}

} // namespace ss
