//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "FileInputStream.h"
#include "StreamConstant.h"

namespace ss
{
FileInputStream::FileInputStream(const CharSequence &file)
{
    new (this) FileInputStream(file.ToStdString().c_str());
}

FileInputStream::FileInputStream(const std::string &file) : filePtr_(nullptr), totalFileSize_(0)
{
    new (this) FileInputStream(file.c_str());
}

FileInputStream::FileInputStream(const char *file) : filePtr_(nullptr), totalFileSize_(0)
{
    filePtr_ = fopen(file, "rb");
    if (filePtr_)
    {
        fseek(filePtr_, 0, SEEK_END);
        totalFileSize_ = ftell(filePtr_);
        fseek(filePtr_, 0, SEEK_SET);
    }
}

FileInputStream::~FileInputStream()
{
    FileInputStream::Close();
}

int FileInputStream::Read()
{
    SSASSERT(filePtr_ != nullptr);
    return fgetc(filePtr_);
}

int32_t FileInputStream::Read(void *buf, uint32_t count)
{
    SSASSERT(filePtr_ != nullptr);
    size_t c = fread(buf, 1, count, filePtr_);
    if (c < count)
    {
        if (feof(filePtr_))
        {
            // EOF
            return c > 0 ? c : StreamConstant::ErrorCode::kEof;
        }
        if (ferror(filePtr_))
        {
            return StreamConstant::ErrorCode::kUnknown; // TODO: a more detailed error code
        }
    }
    return int32_t(c);
}

int64_t FileInputStream::Skip(int64_t n)
{
    SSASSERT(filePtr_ != nullptr);
    if (n < 0)
    {
        return 0;
    }
    int64_t pos = ftell(filePtr_);
    if (0 == fseek(filePtr_, n, SEEK_CUR))
    {
        // Success
        return ftell(filePtr_) - pos;
    }
    return StreamConstant::ErrorCode::kUnknown; // TODO: a more detailed error code
}

int32_t FileInputStream::Available() const
{
    SSASSERT(filePtr_ != nullptr);
    return totalFileSize_ - ftell(filePtr_);
}

void FileInputStream::Close()
{
    if (filePtr_ != nullptr)
    {
        fclose(filePtr_);
        filePtr_ = nullptr;
    }
}

bool FileInputStream::IsValid() const
{
    return filePtr_ != nullptr;
}

std::string FileInputStream::ReadAll()
{
    std::string result;
    result.resize(Available());
    Read(const_cast<char *>(result.data()), result.size());
    return result;
}

int FileInputStream::Seek(int64_t offset, SeekableInputStream::Whence whence)
{
    int wc;
    switch (whence)
    {
    case kSeekCur:
        wc = SEEK_CUR;
        break;
    case kSeekSet:
        wc = SEEK_SET;
        break;
    case kSeekEnd:
        wc = SEEK_END;
        break;
    default:
        SSASSERT(false);
    }
    if (0 == fseek(filePtr_, offset, wc))
    {
        return StreamConstant::ErrorCode::kOk;
    }
    return StreamConstant::ErrorCode::kUnknown;
}

} // namespace ss
