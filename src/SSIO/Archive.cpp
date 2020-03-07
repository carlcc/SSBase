//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Archive.h"
#include "../SSBase/Buffer.h"
#include "../SSBase/Ptr.h"
#include "stream/InputStream.h"
#include "stream/StreamConstant.h"
#include <map>
#include <zip.h>

namespace ss
{

class Archive::ArchiveInputStream : public InputStream
{
    SS_OBJECT(Archive::ArchiveInputStream, InputStream);

public:
    explicit ArchiveInputStream(zip_file_t *zipFile, int64_t available) : zipFile_(zipFile), available_(available)
    {
    }

    ~ArchiveInputStream() override
    {
        Close();
    }

    int Read() override
    {
        uint8_t c;
        auto count = Read(&c, 1);
        if (c == 1)
        {
            return c;
        }
        return count;
    }

    int32_t Read(void *buf, uint32_t count) override
    {
        auto size = zip_fread(zipFile_, buf, count);
        if (size < 0)
        {
            /* ERROR */
            return StreamConstant::ErrorCode::kUnknown;
        }
        if (size == 0)
        {
            return StreamConstant::ErrorCode::kEof;
        }

        available_ -= size;
        return int32_t(size);
    }

    int64_t Skip(int64_t n) override
    {
        return InputStream::Skip(n);
    }

    int32_t Available() const override
    {
        return int32_t(available_);
    }

    void Close() override
    {
        if (zipFile_ != nullptr)
        {
            zip_fclose(zipFile_);
            zipFile_ = nullptr;
        }
    }

    bool IsValid() const override
    {
        return zipFile_ != nullptr;
    }

    zip_file_t *zipFile_;
    int64_t available_;
};

class Archive::Impl
{
public:
    explicit Impl(const std::string &s) : archive_(nullptr), entries_()
    {
        Init(s.c_str());
    }

    explicit Impl(const char *filePath) : archive_(nullptr), entries_()
    {
        Init(filePath);
    }

    void Init(const char *filePath)
    {
        int errorCode = ZIP_ER_OK;
        archive_ = zip_open(filePath, ZIP_RDONLY, &errorCode);
        if (errorCode != ZIP_ER_OK)
        {
            Free();
            return;
        }
    }

    ~Impl()
    {
        Free();
    }

    void Free()
    {
        if (archive_ != nullptr)
        {
            zip_close(archive_);
            archive_ = nullptr;
        }
    }

    const EntryList &GetEntries() const
    {
        if (entries_.empty())
        {
            // If flags is set to ZIP_FL_UNCHANGED, the original number of entries is returned.
            auto numEntries = zip_get_num_entries(archive_, 0);
            if (numEntries > 0)
            {
                entries_.reserve(size_t(numEntries));
                for (int64_t i = 0; i < numEntries; ++i)
                {
                    entries_.emplace_back(zip_get_name(archive_, i, 0));
                }
            }
        }
        return entries_;
    }

    SharedPtr<InputStream> GetInputStream(const CharSequence &file)
    {
        int flag = 0;
        // flag |= ZIP_FL_NOCASE;
        auto index = zip_name_locate(archive_, file.ToStdString().c_str(), flag);
        if (index == -1)
        {
            return nullptr;
        }

        flag = ZIP_STAT_SIZE;
        zip_stat_t stat;
        if (0 != zip_stat_index(archive_, index, flag, &stat))
        {
            return nullptr;
        }
        zip_file_t *zipFile = zip_fopen_index(archive_, index, 0);
        if (zipFile == nullptr)
        {
            return nullptr;
        }
        return MakeShared<ArchiveInputStream>(zipFile, stat.size);
    }

    bool IsValid() const
    {
        return archive_ != nullptr;
    }

    zip_t *archive_;
    mutable EntryList entries_;
};

Archive::Archive(const CharSequence &filePath) : impl_(new Impl(filePath.ToStdString()))
{
}

Archive::Archive(const String &filePath) : impl_(new Impl(filePath.ToStdString()))
{
}

Archive::~Archive()
{
    delete impl_;
}

const Archive::EntryList &Archive::GetEntries() const
{
    return impl_->GetEntries();
}

SharedPtr<InputStream> Archive::GetInputStream(const CharSequence &filePath)
{
    return impl_->GetInputStream(filePath);
}

SharedPtr<InputStream> Archive::GetInputStream(const String &filePath)
{
    return impl_->GetInputStream(filePath);
}

bool Archive::IsValid() const
{
    return impl_->IsValid();
}

} // namespace ss
