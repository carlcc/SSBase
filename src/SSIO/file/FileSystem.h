//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../../SSBase/Buffer.h"
#include "../../SSBase/Str.h"
#include <functional>

namespace ss
{

class FileSystem
{
public:
    const static char kSeparator;
    const static char kInternalSeparator;

    // Return current working directory
    static String GetCWD(bool internalSeparator = true);

    static String GetRelativePath(const CharSequence &path, const CharSequence &relativeTo = String(""),
                                  bool internalSeparator = true);

    static String GetRelativePath(const String &path, const CharSequence &relativeTo = String(""),
                                  bool internalSeparator = true)
    {
        return GetRelativePath((const CharSequence &)path, relativeTo, internalSeparator);
    }

    static String GetRelativePath(const CharSequence &path, const String &relativeTo = String(""),
                                  bool internalSeparator = true)
    {
        return GetRelativePath(path, (const CharSequence &)relativeTo, internalSeparator);
    }

    static String GetRelativePath(const String &path, const String &relativeTo = String(""),
                                  bool internalSeparator = true)
    {
        return GetRelativePath((const CharSequence &)path, (const CharSequence &)relativeTo, internalSeparator);
    }

    static String GetAbsolutePath(const CharSequence &path, bool internalSeparator = true);

    static String GetAbsolutePath(const String &path, bool internalSeparator = true)
    {
        return GetAbsolutePath((const CharSequence &)path, internalSeparator);
    }

    static String GetParent(const CharSequence &path, bool internalSeparator = true)
    {
        return NormalizePath(path + "/..", internalSeparator);
    }

    static String GetParent(const String &path, bool internalSeparator = true)
    {
        return GetParent((const CharSequence &)path, internalSeparator);
    }

    static String MakePath(const CharSequence &prefix, const CharSequence &path, bool internalSeparator = true);

    static String MakePath(const String &prefix, const CharSequence &path, bool internalSeparator = true)
    {
        return MakePath((const CharSequence &)prefix, path, internalSeparator);
    }

    static String MakePath(const CharSequence &prefix, const String &path, bool internalSeparator = true)
    {
        return MakePath(prefix, (const CharSequence &)path, internalSeparator);
    }

    static String MakePath(const String &prefix, const String &path, bool internalSeparator = true)
    {
        return MakePath((const CharSequence &)prefix, (const CharSequence &)path, internalSeparator);
    }

    // Return normalized path
    static String NormalizePath(const CharSequence &path, bool internalSeparator = true);
    static String NormalizePath(const String &path, bool internalSeparator = true)
    {
        return NormalizePath((const CharSequence &)path, internalSeparator);
    }

    static bool IsAbsolutePath(const CharSequence &path);

    static bool IsAbsolutePath(const String &path)
    {
        return IsAbsolutePath((const CharSequence &)path);
    }

    /// This function only split the path with separators, and returns the last part.
    /// i.e.
    /// "../.." returns ".."
    /// "/" returns ""
    static String GetFileName(const CharSequence &path);
    static String GetFileName(const String &path)
    {
        return GetFileName((const CharSequence &)path);
    }

    /// This function first invoke GetFileName to get the file's name, then
    /// split the name with dot(.), and returns the last part. Returns empty string if there is no dot
    /// i.e.
    /// "../.." returns ""
    /// "a/b.txt.gz" return "gz"
    /// "../index" returns ""
    static String GetFileExtName(const CharSequence &path);
    static String GetFileExtName(const String &path)
    {
        return GetFileExtName((const CharSequence &)path);
    }

    static bool IsFile(const CharSequence &path);
    static bool IsFile(const String &path)
    {
        return IsFile((const CharSequence &)path);
    }

    static bool IsDirectory(const CharSequence &path);
    static bool IsDirectory(const String &path)
    {
        return IsDirectory((const CharSequence &)path);
    }

    static bool IsHidden(const CharSequence &path);
    static bool IsHidden(const String &path)
    {
        return IsHidden((const CharSequence &)path);
    }

    static bool Exists(const CharSequence &path);
    static bool Exists(const String &path)
    {
        return Exists((const CharSequence &)path);
    }

    static bool MakeFile(const CharSequence &path);
    static bool MakeFile(const String &path)
    {
        return MakeFile((const CharSequence &)path);
    }

    static bool MakeDir(const CharSequence &path);
    static bool MakeDir(const String &path)
    {
        return MakeDir((const CharSequence &)path);
    }

    static bool MakeDirs(const CharSequence &path);
    static bool MakeDirs(const String &path)
    {
        return MakeDirs((const CharSequence &)path);
    }

    static bool Rename(const CharSequence &from, const CharSequence &to);
    static bool Rename(const String &from, const CharSequence &to)
    {
        return Rename((const CharSequence &)from, to);
    }
    static bool Rename(const CharSequence &from, const String &to)
    {
        return Rename(from, (const CharSequence &)to);
    }
    static bool Rename(const String &from, const String &to)
    {
        return Rename((const CharSequence &)from, (const CharSequence &)to);
    }

    static bool Delete(const CharSequence &path, bool recursive = false);
    static bool Delete(const String &path, bool recursive = false)
    {
        return Delete((const CharSequence &)path, recursive);
    }

    using FilePathFilter = std::function<bool(const CharSequence &)>;
    /// The file path will be saved if the filter test returns true
    static std::vector<String> ListFiles(const CharSequence &path, bool internalSeparator = true,
                                         const FilePathFilter &filter = nullptr);
    static std::vector<String> ListFiles(const String &path, bool internalSeparator = true,
                                         const FilePathFilter &filter = nullptr)
    {
        return ListFiles((const CharSequence &)path, internalSeparator, filter);
    }

    using FileNameFilter = std::function<bool(const CharSequence &)>;
    /// This file only lists the file names under specified path, if you want to get all the full paths,
    /// you may need to concat it by yourself, or invoke ListFiles.
    /// The file path will be saved if the filter test returns true
    static std::vector<String> ListFileNames(const CharSequence &path, const FileNameFilter &filter = nullptr);
    static std::vector<String> ListFileNames(const String &path, const FileNameFilter &filter = nullptr)
    {
        return ListFileNames((const CharSequence &)path, filter);
    }

private:
    static String RemoveDotDotInternal(const CharSequence &path, char separator);
    static bool IsSeparatorInternal(CharSequence::CharType c);

    static void NormalizeSeparatorInternal(String &path, char separator);
};

} // namespace ss