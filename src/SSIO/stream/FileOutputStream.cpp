//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "FileOutputStream.h"
#include "StreamConstant.h"

namespace ss
{

FileOutputStream::FileOutputStream(const CharSequence &file) : filePtr_(nullptr)
{
    new (this) FileOutputStream(file.ToStdString().c_str());
}

FileOutputStream::FileOutputStream(const std::string &file) : filePtr_(nullptr)
{
    new (this) FileOutputStream(file.c_str());
}

FileOutputStream::FileOutputStream(const char *file) : filePtr_(nullptr)
{
    filePtr_ = fopen(file, "wb");
}

FileOutputStream::~FileOutputStream()
{
    FileOutputStream::Close();
}

int FileOutputStream::Write(uint8_t byte)
{
    SSASSERT(filePtr_ != nullptr);
    return fputc(byte, filePtr_);
}

int32_t FileOutputStream::Write(const void *data, uint32_t count)
{
    SSASSERT(filePtr_ != nullptr);
    size_t c = fwrite(data, 1, count, filePtr_);
    if (c < count)
    {
        return StreamConstant::ErrorCode::kUnknown;
    }
    return c;
}

void FileOutputStream::Close()
{
    if (filePtr_ != nullptr)
    {
        Flush();
        fclose(filePtr_);
        filePtr_ = nullptr;
    }
}

bool FileOutputStream::IsValid() const
{
    return filePtr_ != nullptr;
}

int32_t FileOutputStream::Flush()
{
    if (fflush(filePtr_) == 0)
    {
        return StreamConstant::ErrorCode::kOk;
    }
    return StreamConstant::ErrorCode::kUnknown;
}

} // namespace ss
