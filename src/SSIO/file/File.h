//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../../SSBase/Str.h"

namespace ss {

class File {
public:
    // This constructor returns a CWD
    File();
    explicit File(const String& path);
    explicit File(const CharSequence& path);
    File(const CharSequence& parent, const CharSequence& path);
    File(const String& parent, const CharSequence& path);
    File(const CharSequence& parent, const String& path);
    File(const String& parent, const String& path);
    File(const File& parent, const String& path);
    File(const File& parent, const CharSequence& path);

    File(const File&) = default;
    File(File&&) = default;

    File& operator=(const File&) = default;
    File& operator=(File&&) = default;

    String GetRelativePath(const String& relativeTo) const;
    String GetRelativePath(const CharSequence& relativeTo) const;
    String GetRelativePath(const File& relativeTo) const;
    // Relative to CWD
    String GetRelativePath() const;

    String GetAbsolutePath() const;

    File GetAbsoluteFile() const;

    String GetParentPath() const;

    File GetParent() const;

    bool IsAbsoluteFile() const;

    bool IsRegularFile() const;

    bool IsDirectory();

    bool Exists() const;

    bool IsHidden() const;

    bool MakeDir() const;

    bool MakeDirs() const;

    bool MoveTo(const String& dest);

    bool MoveTo(const File& dest);

    bool Delete(bool recursive = false);

    String GetPath() const;

    String GetName() const;

    String GetExt() const;

    using FilePathFilter = std::function<bool(const CharSequence&)>;
    std::vector<String> List(const FilePathFilter& filter = nullptr) const;

    using FileFilter = std::function<bool(const File&)>;
    std::vector<File> ListFiles(const FileFilter& filter = nullptr) const;

private:
    String path_;
};

}