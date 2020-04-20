//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "File.h"
#include "FileSystem.h"

namespace ss {

using FS = FileSystem;
const static bool kFileUseInternalSeparator = true;

File::File()
    : path_(FS::GetCWD(kFileUseInternalSeparator))
{
}

File::File(const String& path)
    : path_(FS::NormalizePath(path, kFileUseInternalSeparator))
{
}

File::File(const CharSequence& path)
    : path_(FS::NormalizePath(path, kFileUseInternalSeparator))
{
}

File::File(const CharSequence& parent, const CharSequence& path)
    : path_(FS::NormalizePath(FS::MakePath(parent, path), kFileUseInternalSeparator))
{
}

File::File(const String& parent, const CharSequence& path)
    : path_(FS::NormalizePath(FS::MakePath(parent, path), kFileUseInternalSeparator))
{
}

File::File(const CharSequence& parent, const String& path)
    : path_(FS::NormalizePath(FS::MakePath(parent, path), kFileUseInternalSeparator))
{
}

File::File(const String& parent, const String& path)
    : path_(FS::NormalizePath(FS::MakePath(parent, path), kFileUseInternalSeparator))
{
}

File::File(const File& parent, const String& path)
    : path_(FS::NormalizePath(FS::MakePath(parent.GetPath(), path), kFileUseInternalSeparator))
{
}

File::File(const File& parent, const CharSequence& path)
    : path_(FS::NormalizePath(FS::MakePath(parent.GetPath(), path), kFileUseInternalSeparator))
{
}

String File::GetRelativePath(const String& relativeTo) const
{
    return FS::GetRelativePath(GetPath(), relativeTo, kFileUseInternalSeparator);
}

String File::GetRelativePath(const CharSequence& relativeTo) const
{
    return FS::GetRelativePath(GetPath(), relativeTo, kFileUseInternalSeparator);
}

String File::GetRelativePath(const File& relativeTo) const
{
    return FS::GetRelativePath(GetPath(), relativeTo.GetPath(), kFileUseInternalSeparator);
}

String File::GetRelativePath() const
{
    return FS::GetRelativePath(GetPath(), "", kFileUseInternalSeparator);
}

String File::GetAbsolutePath() const
{
    if (IsAbsoluteFile()) {
        return path_;
    }
    return FS::GetAbsolutePath(GetPath(), kFileUseInternalSeparator);
}

File File::GetAbsoluteFile() const
{
    if (IsAbsoluteFile()) {
        return *this;
    }
    return File(GetAbsolutePath());
}

String File::GetParentPath() const
{
    return FS::GetParent(GetPath(), kFileUseInternalSeparator);
}

File File::GetParent() const
{
    return File(FS::GetParent(GetPath(), kFileUseInternalSeparator));
}

bool File::IsAbsoluteFile() const
{
    return FS::IsAbsolutePath(path_);
}

bool File::IsRegularFile() const
{
    return FS::IsFile(GetPath());
}

bool File::IsDirectory()
{
    return FS::IsDirectory(GetPath());
}

bool File::Exists() const
{
    return FS::Exists(GetPath());
}

bool File::IsHidden() const
{
    return FS::IsHidden(GetPath());
}

bool File::MakeDir() const
{
    return FS::MakeDir(GetPath());
}

bool File::MakeDirs() const
{
    return FS::MakeDirs(GetPath());
}

bool File::MoveTo(const String& dest)
{
    return FS::Rename(GetPath(), dest);
}

bool File::MoveTo(const File& dest)
{
    return FS::Rename(GetPath(), dest.GetPath());
}

bool File::Delete(bool recursive)
{
    return FS::Delete(GetPath(), recursive);
}

String File::GetName() const
{
    return FS::GetFileName(GetPath());
}

String File::GetExt() const
{
    return FS::GetFileExtName(GetPath());
}

String File::GetPath() const
{
    return path_;
}

std::vector<String> File::List(const File::FilePathFilter& filter) const
{
    return FS::ListFileNames(GetPath(), filter);
}

std::vector<File> File::ListFiles(const File::FileFilter& filter) const
{
    std::vector<File> files;
    FS::ListFileNames(GetPath(), [this, &files, &filter](const CharSequence& name) -> bool {
        File f(*this, name);
        if (filter == nullptr || filter(f)) {
            files.push_back(std::move(f));
        }
        // always return false since we are not interested in the return value of FS::ListFileNames
        return false;
    });
    return files;
}

}
