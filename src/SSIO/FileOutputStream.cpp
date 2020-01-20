//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "FileOutputStream.h"
#include "StreamConstant.h"

namespace ss
{

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

int32_t FileOutputStream::Write(void *buf, uint32_t count)
{
    SSASSERT(filePtr_ != nullptr);
    size_t c = fwrite(buf, 1, count, filePtr_);
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
        fclose(filePtr_);
        filePtr_ = nullptr;
    }
}

bool FileOutputStream::IsValid() const
{
    return filePtr_ != nullptr;
}

} // namespace ss
