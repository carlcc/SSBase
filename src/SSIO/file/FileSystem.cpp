//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//
#include "FileSystem.h"

#ifdef SS_PLAFORM_WIN32
#error not yet implemented
#elif defined(SS_PLATFORM_UNIX)
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace ss
{

#ifdef SS_PLATFORM_WIN32
const char FileSystem::kSeparator = '\\';
#else
const char FileSystem::kSeparator = '/';
#endif

const char FileSystem::kInternalSeparator = '/';

// Return current working directory
String FileSystem::GetCWD(bool internalSeparator)
{
    String result;

#ifdef SS_PLAFORM_WIN32
#error Not yet implemented
#else
    char buffer[4096];
    char *p = getcwd(buffer, sizeof(buffer));
    result = buffer;
    return NormalizePath(String(p), internalSeparator);
#endif
}

String FileSystem::GetRelativePath(const CharSequence &path, const CharSequence &relativeTo, bool internalSeparator)
{
    String absolutePath = GetAbsolutePath(path);
    String relativeToAbsolute = relativeTo.Empty() ? GetCWD() : GetAbsolutePath(relativeTo);
    std::vector<StringView> paths = absolutePath.Split(kInternalSeparator);
    std::vector<StringView> relativeToPaths = relativeToAbsolute.Split(kInternalSeparator);

    size_t index = 0;
    while (index < paths.size() && index < relativeToPaths.size())
    {
        if (paths[index] != relativeToPaths[index])
        {
            break;
        }
        ++index;
    }

    // path       = /a/b/c/d/e
    // relativeTo = /a/b/f/g/h
    // result     = ../../../c/d/e
    String result;
    size_t numDotDot = relativeToPaths.size() - index;

    for (size_t i = 0; i < numDotDot; ++i)
    {
        result += "..";
        result += internalSeparator ? kInternalSeparator : kSeparator;
    }

    size_t numDirs = paths.size() - index;
    for (size_t i = 0; i < numDirs; ++i)
    {
        result += paths[i + index];
        result += internalSeparator ? kInternalSeparator : kSeparator;
    }

    if (result.Empty())
    {
        return ".";
    }
    result.Resize(result.Length() - 1); // Delete the last separator
    return result;
}

String FileSystem::GetAbsolutePath(const CharSequence &path, bool internalSeparator)
{
    if (IsAbsolutePath(path))
    {
        return NormalizePath(path, internalSeparator);
    }
    else
    {
        return NormalizePath(GetCWD() + kSeparator + path, internalSeparator);
    }
}

String FileSystem::MakePath(const CharSequence &prefix, const CharSequence &path, bool internalSeparator)
{
    return NormalizePath(prefix + kInternalSeparator + path, internalSeparator);
}

// Return normalized path
String FileSystem::NormalizePath(const CharSequence &path, bool internalSeparator)
{
    if (path.Empty())
    {
        return "";
    }
    char separator = internalSeparator ? kInternalSeparator : kSeparator;
    String p = path;

    NormalizeSeparatorInternal(p, separator);
    if (IsAbsolutePath(p))
    {
#ifdef SS_PLAFORM_WIN32
#error Not yet implemented
        return p.SubStringView(0, 2) + separator + RemoveDotDotInternal(p.SubStringView(3), separator);
#else
        return separator + RemoveDotDotInternal(p.SubStringView(1), separator);
#endif
    }
    else
    {
        String result = RemoveDotDotInternal(p, separator);
        if (result.Empty())
        {
            result += ".";
        }
        return result;
    }
}

bool FileSystem::IsAbsolutePath(const CharSequence &path)
{
#ifdef SS_PLATFORM_WIN32
    return path.Length() >= 3 && String::IsAsciiAlpha(path[0]) && path[1] == ':' && IsSeparatorInternal(path[2]);
#else
    return path.Length() >= 1 && IsSeparatorInternal(path[0]);
#endif
}

String FileSystem::GetFileName(const CharSequence &path)
{
    auto index = path.RFind(kSeparator);
    if (kSeparator != kInternalSeparator)
    {
        auto index2 = path.RFind(kInternalSeparator);
        if (index == String::kNPos)
        {
            if (index2 == String::kNPos)
            {
                // No separator is found
                return path;
            }
            else
            {
                index = index2;
            }
        }
        else
        {
            if (index2 != String::kNPos)
            {
                // No separator is found
                if (index2 > index)
                {
                    index = index2;
                }
            }
        }
    }
    else
    {
        if (index == String::kNPos)
        {
            return path;
        }
    }

    return path.SubString(index + 1);
}

String FileSystem::GetFileExtName(const CharSequence &path)
{
    String name = GetFileName(path);
    auto index = name.RFind(".");
    if (index == String::kNPos)
    {
        return "";
    }
    return name.SubString(index + 1);
}

/// FIle op
bool FileSystem::IsFile(const CharSequence &path)
{
#ifdef SS_PLATFORM_UNIX
    struct stat statBuf;
    stat(path.ToStdString().c_str(), &statBuf);
    return S_ISREG(statBuf.st_mode);
#else
#error not yet implemented
#endif
}

bool FileSystem::IsDirectory(const CharSequence &path)
{
#ifdef SS_PLATFORM_UNIX
    struct stat statBuf;
    stat(path.ToStdString().c_str(), &statBuf);
    return S_ISDIR(statBuf.st_mode);
#else
#error not yet implemented
#endif
}

bool FileSystem::IsHidden(const CharSequence &path)
{
#ifdef SS_PLATFORM_UNIX
    return GetFileName(path).StartsWith(".");
#else
#error not yet implemented
#endif
}

bool FileSystem::Exists(const CharSequence &path)
{
#ifdef SS_PLATFORM_UNIX
    return access(path.ToStdString().c_str(), F_OK) != -1;
#else
#error not yet implemented
#endif
}

bool FileSystem::MakeFile(const CharSequence &path)
{
    FILE *f = fopen(path.ToStdString().c_str(), "a");
    if (f != nullptr)
    {
        fclose(f);
        return true;
    }
    return false;
}

bool FileSystem::MakeDir(const CharSequence &path)
{
#ifdef SS_PLATFORM_UNIX
    return 0 == mkdir(path.ToStdString().c_str(), 0755);
#else
#error not yet implemented
#endif
}

bool FileSystem::MakeDirs(const CharSequence &path)
{
    String fullPath = GetAbsolutePath(path);
    uint32_t index = String::kNPos;

    const static String kSeparatorString(kInternalSeparator);
    do
    {
        StringView parent = path.SubStringView(0, index);
        if (Exists(parent))
        {
            if (!IsDirectory(parent))
            {
                return false; // one of the parent path is not a directory
            }
            break;
        }
        index = path.RFind(kSeparatorString, index);
    } while (index != String::kNPos);

    if (index == String::kNPos)
    {
        return MakeDir(path);
    }

    do
    {
        index = path.Find(kSeparatorString, index + 1);
        StringView dir = path.SubStringView(0, index);
        if (!MakeDir(dir))
        {
            return false;
        }
    } while (index != String::kNPos);
    return true;
}

bool FileSystem::Rename(const CharSequence &from, const CharSequence &to)
{
    return rename(from.ToStdString().c_str(), to.ToStdString().c_str()) == 0;
}

bool FileSystem::Delete(const CharSequence &path, bool recursive)
{
    if (recursive && IsDirectory(path))
    {
        std::vector<String> files = ListFiles(path);
        for (auto &f : files)
        {
            if (!Delete(f, true))
            {
                return false;
            }
        }
    }

    std::cout << "--- " << path << std::endl;
    return remove(path.ToStdString().c_str()) == 0;
}

std::vector<String> FileSystem::ListFiles(const CharSequence &path, bool internalSeparator,
                                          const FileSystem::FilePathFilter &filter)
{
#ifdef SS_PLATFORM_UNIX
    std::vector<String> result;

    const char separator = internalSeparator ? kInternalSeparator : kSeparator;
    String normalizedPath = NormalizePath(path, internalSeparator);

    DIR *dir = opendir(path.ToStdString().c_str());
    if (dir == nullptr)
    {
        return result;
    }

    while (true)
    {
        dirent *ent = readdir(dir);
        if (ent == nullptr)
        {
            break;
        }
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue;
        }
        String name = normalizedPath + separator + ent->d_name;
        if (filter != nullptr && !filter(name))
        {
            continue;
        }
        result.push_back(std::move(name));
    }
    closedir(dir);
    return result;
#else
#error not yet implemented
#endif
}

std::vector<String> FileSystem::ListFileNames(const CharSequence &path, const FileSystem::FileNameFilter &filter)
{
#ifdef SS_PLATFORM_UNIX
    std::vector<String> result;

    DIR *dir = opendir(path.ToStdString().c_str());
    if (dir == nullptr)
    {
        return result;
    }

    while (true)
    {
        dirent *ent = readdir(dir);
        if (ent == nullptr)
        {
            break;
        }

        String name = ent->d_name;
        if (filter != nullptr && !filter(name))
        {
            continue;
        }
        result.push_back(std::move(name));
    }
    closedir(dir);
    return result;
#else
#error not yet implemented
#endif
}

String FileSystem::RemoveDotDotInternal(const CharSequence &path, char separator)
{
    std::vector<StringView> paths = path.Split(separator);
    std::vector<const CharSequence *> stack;
    const String kDotDot = "..";

    for (auto &sv : paths)
    {
        if (sv == "..")
        {
            if (!stack.empty() && *stack[stack.size() - 1] != "..")
            {
                stack.pop_back();
            }
            else
            {
                stack.push_back(&kDotDot);
            }
        }
        else if (sv == ".")
        {
        }
        else
        {
            stack.push_back(&sv);
        }
    }

    String result;
    if (!stack.empty())
    {
        auto it = stack.begin();
        result += **it;
        for (++it; it < stack.end(); ++it)
        {
            result += separator;
            result += **it;
        }
    }
    return result;
}
bool FileSystem::IsSeparatorInternal(CharSequence::CharType c)
{
    return c == '/' || c == '\\';
}

void FileSystem::NormalizeSeparatorInternal(String &path, char separator)
{
    uint32_t i = 0, j = 0;
    while (j < path.Length())
    {
        if (IsSeparatorInternal(path[j]))
        {
            path[i] = separator;
            ++i;
            ++j;
            // Skip the rest continuous separators
            while (j < path.Length() && IsSeparatorInternal(path[j]))
            {
                ++j;
            }
        }
        else
        {
            path[i] = path[j];
            ++i;
            ++j;
        }
    }
    SSASSERT(i > 0);
    if (path.EndsWith(separator))
        --i;
    path.Resize(i);
}

} // namespace ss