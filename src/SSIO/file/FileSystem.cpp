//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//
#include "FileSystem.h"

#ifdef SS_PLATFORM_WIN32
#include <Windows.h>
#include <direct.h>
#include <io.h>
#ifdef __GNUC__
#include <sys/stat.h>
#endif
#elif defined(SS_PLATFORM_UNIX)
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace ss {

#ifdef SS_PLATFORM_WIN32
const char FileSystem::kSeparator = '\\';
#else
const char FileSystem::kSeparator = '/';
#endif

const char FileSystem::kInternalSeparator = '/';

// Return current working directory
String FileSystem::GetCWD(bool internalSeparator)
{
#ifdef SS_PLATFORM_WIN32
    wchar_t buffer[4096];
    wchar_t* p = _wgetcwd(buffer, sizeof(buffer) / sizeof(buffer[0]));
#else
    char buffer[4096];
    char* p = getcwd(buffer, sizeof(buffer) / sizeof(buffer[0]));
#endif
    return NormalizePath(String(p), internalSeparator);
}

String FileSystem::GetRelativePath(const CharSequence& path, const CharSequence& relativeTo, bool internalSeparator)
{
    String absolutePath = GetAbsolutePath(path);
    String relativeToAbsolute = relativeTo.Empty() ? GetCWD() : GetAbsolutePath(relativeTo);
    std::vector<StringView> paths = absolutePath.Split(kInternalSeparator);
    std::vector<StringView> relativeToPaths = relativeToAbsolute.Split(kInternalSeparator);

    size_t index = 0;
    while (index < paths.size() && index < relativeToPaths.size()) {
        if (paths[index] != relativeToPaths[index]) {
            break;
        }
        ++index;
    }

    // path       = /a/b/c/d/e
    // relativeTo = /a/b/f/g/h
    // result     = ../../../c/d/e
    String result;
    size_t numDotDot = relativeToPaths.size() - index;

    for (size_t i = 0; i < numDotDot; ++i) {
        result += "..";
        result += internalSeparator ? kInternalSeparator : kSeparator;
    }

    size_t numDirs = paths.size() - index;
    for (size_t i = 0; i < numDirs; ++i) {
        result += paths[i + index];
        result += internalSeparator ? kInternalSeparator : kSeparator;
    }

    if (result.Empty()) {
        return ".";
    }
    result.Resize(result.Length() - 1); // Delete the last separator
    return result;
}

String FileSystem::GetAbsolutePath(const CharSequence& path, bool internalSeparator)
{
    if (IsAbsolutePath(path)) {
        return NormalizePath(path, internalSeparator);
    } else {
        return NormalizePath(GetCWD() + kSeparator + path, internalSeparator);
    }
}

String FileSystem::MakePath(const CharSequence& prefix, const CharSequence& path, bool internalSeparator)
{
    return NormalizePath(prefix + kInternalSeparator + path, internalSeparator);
}

// Return normalized path
String FileSystem::NormalizePath(const CharSequence& path, bool internalSeparator)
{
    if (path.Empty()) {
        return "";
    }
    char separator = internalSeparator ? kInternalSeparator : kSeparator;
    String p = path;

    NormalizeSeparatorInternal(p, separator);
    if (IsAbsolutePath(p)) {
#ifdef SS_PLATFORM_WIN32
        return p.SubStringView(0, 2) + separator + RemoveDotDotInternal(p.SubStringView(3), separator);
#else
        return separator + RemoveDotDotInternal(p.SubStringView(1), separator);
#endif
    } else {
        String result = RemoveDotDotInternal(p, separator);
        if (result.Empty()) {
            result += ".";
        }
        return result;
    }
}

bool FileSystem::IsAbsolutePath(const CharSequence& path)
{
#ifdef SS_PLATFORM_WIN32
    return path.Length() >= 3 && String::IsAsciiAlpha(path[0]) && path[1].code == ':' && IsSeparatorInternal(path[2]);
#else
    return path.Length() >= 1 && IsSeparatorInternal(path[0]);
#endif
}

String FileSystem::GetFileName(const CharSequence& path)
{
    auto index = path.RFind(kSeparator);
    if (kSeparator != kInternalSeparator) {
        auto index2 = path.RFind(kInternalSeparator);
        if (index == String::kNPos) {
            if (index2 == String::kNPos) {
                // No separator is found
                return path;
            } else {
                index = index2;
            }
        } else {
            if (index2 != String::kNPos) {
                // No separator is found
                if (index2 > index) {
                    index = index2;
                }
            }
        }
    } else {
        if (index == String::kNPos) {
            return path;
        }
    }

    return path.SubString(index + 1);
}

String FileSystem::GetFileExtName(const CharSequence& path)
{
    String name = GetFileName(path);
    auto index = name.RFind(".");
    if (index == String::kNPos) {
        return "";
    }
    return name.SubString(index + 1);
}

/// FIle op
bool FileSystem::IsFile(const CharSequence& path)
{
#ifdef SS_PLATFORM_UNIX
    struct stat statBuf;
    stat(path.ToStdString().c_str(), &statBuf);
    return S_ISREG(statBuf.st_mode);
#else
    struct _stat statBuf;
    _wstat(path.ToStdWString().c_str(), &statBuf);
    return _S_IFREG & statBuf.st_mode;
#endif
}

bool FileSystem::IsDirectory(const CharSequence& path)
{
#ifdef SS_PLATFORM_UNIX
    struct stat statBuf;
    stat(path.ToStdString().c_str(), &statBuf);
    return S_ISDIR(statBuf.st_mode);
#else
    struct _stat statBuf;
    _wstat(path.ToStdWString().c_str(), &statBuf);
    return _S_IFDIR & statBuf.st_mode;
#endif
}

bool FileSystem::IsHidden(const CharSequence& path)
{
#ifdef SS_PLATFORM_UNIX
    return GetFileName(path).StartsWith(".");
#else
    DWORD attributes = GetFileAttributesW(path.ToStdWString().c_str());
    return attributes & FILE_ATTRIBUTE_HIDDEN;
#endif
}

bool FileSystem::Exists(const CharSequence& path)
{
#ifdef SS_PLATFORM_UNIX
    return access(path.ToStdString().c_str(), F_OK) != -1;
#else
    return _waccess(path.ToStdWString().c_str(), 0) != -1;
#endif
}

bool FileSystem::MakeFile(const CharSequence& path)
{
    if (Exists(path)) {
        return false;
    }
#ifdef SS_PLATFORM_UNIX
    FILE* f = fopen(path.ToStdString().c_str(), "a");
#else
    FILE* f = _wfopen(path.ToStdWString().c_str(), L"a");
#endif // SS_PLATFORM_UNIX

    if (f != nullptr) {
        fclose(f);
        return true;
    }
    return false;
}

bool FileSystem::MakeDir(const CharSequence& path)
{
#ifdef SS_PLATFORM_UNIX
    return 0 == mkdir(path.ToStdString().c_str(), 0755);
#else
    return 0 == _wmkdir(path.ToStdWString().c_str());
#endif
}

bool FileSystem::MakeDirs(const CharSequence& path)
{
    String fullPath = GetAbsolutePath(path);
    uint32_t index = String::kNPos;

    const static String kSeparatorString(kInternalSeparator);
    do {
        StringView parent = path.SubStringView(0, index);
        if (Exists(parent)) {
            if (!IsDirectory(parent)) {
                return false; // one of the parent path is not a directory
            }
            break;
        }
        index = path.RFind(kSeparatorString, index);
    } while (index != String::kNPos);

    if (index == String::kNPos) {
        return MakeDir(path);
    }

    do {
        index = path.Find(kSeparatorString, index + 1);
        StringView dir = path.SubStringView(0, index);
        if (!MakeDir(dir)) {
            return false;
        }
    } while (index != String::kNPos);
    return true;
}

bool FileSystem::Rename(const CharSequence& from, const CharSequence& to)
{
#ifdef SS_PLATFORM_WIN32
    return _wrename(from.ToStdWString().c_str(), to.ToStdWString().c_str()) == 0;
#else
    return rename(from.ToStdString().c_str(), to.ToStdString().c_str()) == 0;
#endif
}

bool FileSystem::Delete(const CharSequence& path, bool recursive)
{
    if (IsDirectory(path)) {
        if (recursive) {
            std::string p = path.ToStdString();
            std::vector<String> files = ListFiles(path);
            for (auto& f : files) {
                if (!Delete(f, true)) {
                    return false;
                }
            }
        }
#ifdef SS_PLATFORM_UNIX
        return remove(path.ToStdString().c_str()) == 0;
#else
        return RemoveDirectoryW(path.ToStdWString().c_str());
#endif
    } else {
#ifdef SS_PLATFORM_UNIX
        return remove(path.ToStdString().c_str()) == 0;
#else
        return DeleteFileW(path.ToStdWString().c_str());
#endif
    }
}

std::vector<String> FileSystem::ListFiles(const CharSequence& path, bool internalSeparator,
    const FileSystem::FilePathFilter& filter)
{
    const char separator = internalSeparator ? kInternalSeparator : kSeparator;
    String normalizedPath = NormalizePath(path, internalSeparator);

#ifdef SS_PLATFORM_UNIX
    std::vector<String> result;

    DIR* dir = opendir(path.ToStdString().c_str());
    if (dir == nullptr) {
        return result;
    }

    while (true) {
        dirent* ent = readdir(dir);
        if (ent == nullptr) {
            break;
        }
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        String name = normalizedPath + separator + ent->d_name;
        if (filter != nullptr && !filter(name)) {
            continue;
        }
        result.push_back(std::move(name));
    }
    closedir(dir);
    return result;
#else
    WIN32_FIND_DATAW fdFile;
    HANDLE hFind = NULL;

    //Specify a file mask. *.* = We want everything!
    std::wstring pattern = (normalizedPath + "\\*").ToStdWString();

    std::vector<String> result;
    if ((hFind = FindFirstFileW(pattern.c_str(), &fdFile)) == INVALID_HANDLE_VALUE) {
        return result;
    }

    for (bool run = true; run; run = FindNextFileW(hFind, &fdFile)) {
        if (wcscmp(fdFile.cFileName, L".") == 0 || wcscmp(fdFile.cFileName, L"..") == 0) {
            continue;
        }
        String name = normalizedPath + separator + fdFile.cFileName;
        if (filter != nullptr && !filter(name)) {
            continue;
        }
        result.push_back(std::move(name));
    }

    FindClose(hFind);

    return result;
#endif
}

std::vector<String> FileSystem::ListFileNames(const CharSequence& path, const FileSystem::FileNameFilter& filter)
{
#ifdef SS_PLATFORM_UNIX
    std::vector<String> result;

    DIR* dir = opendir(path.ToStdString().c_str());
    if (dir == nullptr) {
        return result;
    }

    while (true) {
        dirent* ent = readdir(dir);
        if (ent == nullptr) {
            break;
        }

        String name = ent->d_name;
        if (filter != nullptr && !filter(name)) {
            continue;
        }
        result.push_back(std::move(name));
    }
    closedir(dir);
    return result;
#else
    WIN32_FIND_DATAW fdFile;
    HANDLE hFind = NULL;

    //Specify a file mask. *.* = We want everything!
    std::wstring pattern = (path + "\\*").ToStdWString();

    std::vector<String> result;
    if ((hFind = FindFirstFileW(pattern.c_str(), &fdFile)) == INVALID_HANDLE_VALUE) {
        return result;
    }

    for (bool run = true; run; run = FindNextFileW(hFind, &fdFile)) {
        if (wcscmp(fdFile.cFileName, L".") == 0 || wcscmp(fdFile.cFileName, L"..") == 0) {
            continue;
        }
        String name = fdFile.cFileName;
        if (filter != nullptr && !filter(name)) {
            continue;
        }
        result.push_back(std::move(name));
    }

    FindClose(hFind);

    return result;
#endif
}

String FileSystem::RemoveDotDotInternal(const CharSequence& path, char separator)
{
    std::vector<StringView> paths = path.Split(separator);
    std::vector<const CharSequence*> stack;
    const String kDotDot = "..";

    for (auto& sv : paths) {
        if (sv == "..") {
            if (!stack.empty() && *stack[stack.size() - 1] != "..") {
                stack.pop_back();
            } else {
                stack.push_back(&kDotDot);
            }
        } else if (sv == ".") {
        } else {
            stack.push_back(&sv);
        }
    }

    String result;
    if (!stack.empty()) {
        auto it = stack.begin();
        result += **it;
        for (++it; it < stack.end(); ++it) {
            result += separator;
            result += **it;
        }
    }
    return result;
}
bool FileSystem::IsSeparatorInternal(CharSequence::CharType c)
{
    return c.code == '/' || c.code == '\\';
}

void FileSystem::NormalizeSeparatorInternal(String& path, char separator)
{
    uint32_t i = 0, j = 0;
    while (j < path.Length()) {
        if (IsSeparatorInternal(path[j])) {
            path[i].code = separator;
            ++i;
            ++j;
            // Skip the rest continuous separators
            while (j < path.Length() && IsSeparatorInternal(path[j])) {
                ++j;
            }
        } else {
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