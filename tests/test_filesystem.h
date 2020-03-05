#include <SSIO/file/FileSystem.h>

namespace TestFileSystem
{

void test_relative_path()
{
    using namespace ss;
    {
        String path1 = "a/b/c/d/e";
        String path2 = "a/b/f/g";
        String path1RelativeTo2 = FileSystem::GetRelativePath(path1, path2);
        String path2RelativeTo1 = FileSystem::GetRelativePath(path2, path1);
        SSASSERT(path1RelativeTo2 == "../../c/d/e");
        SSASSERT(path2RelativeTo1 == "../../../f/g");

        SSASSERT(FileSystem::NormalizePath(path2 + '/' + path1RelativeTo2) == FileSystem::NormalizePath(path1));
        SSASSERT(FileSystem::NormalizePath(path1 + '/' + path2RelativeTo1) == FileSystem::NormalizePath(path2));
        SSASSERT(FileSystem::GetAbsolutePath(path2 + '/' + path1RelativeTo2) == FileSystem::GetAbsolutePath(path1));
        SSASSERT(FileSystem::GetAbsolutePath(path1 + '/' + path2RelativeTo1) == FileSystem::GetAbsolutePath(path2));
    }
    {
        String path1 = "/a/b\\//c/d/e";
        String path2 = "/a/b/f/g";
        String path1RelativeTo2 = FileSystem::GetRelativePath(path1, path2);
        String path2RelativeTo1 = FileSystem::GetRelativePath(path2, path1);
        SSASSERT(path1RelativeTo2 == "../../c/d/e");
        SSASSERT(path2RelativeTo1 == "../../../f/g");
        SSASSERT(FileSystem::NormalizePath(path2 + '/' + path1RelativeTo2) == FileSystem::NormalizePath(path1));
        SSASSERT(FileSystem::NormalizePath(path1 + '/' + path2RelativeTo1) == FileSystem::NormalizePath(path2));
        SSASSERT(FileSystem::GetAbsolutePath(path2 + '/' + path1RelativeTo2) == FileSystem::GetAbsolutePath(path1));
        SSASSERT(FileSystem::GetAbsolutePath(path1 + '/' + path2RelativeTo1) == FileSystem::GetAbsolutePath(path2));

        path1 = "/d/e";
        path2 = "/f/g";
        path1RelativeTo2 = FileSystem::GetRelativePath(path1, path2);
        path2RelativeTo1 = FileSystem::GetRelativePath(path2, path1);
        SSASSERT(path1RelativeTo2 == "../../d/e");
        SSASSERT(path2RelativeTo1 == "../../f/g");
        SSASSERT(FileSystem::NormalizePath(path2 + '/' + path1RelativeTo2) == FileSystem::NormalizePath(path1));
        SSASSERT(FileSystem::NormalizePath(path1 + '/' + path2RelativeTo1) == FileSystem::NormalizePath(path2));
        SSASSERT(FileSystem::GetAbsolutePath(path2 + '/' + path1RelativeTo2) == FileSystem::GetAbsolutePath(path1));
        SSASSERT(FileSystem::GetAbsolutePath(path1 + '/' + path2RelativeTo1) == FileSystem::GetAbsolutePath(path2));

        path1 = "/f/g";
        path2 = "/f/g";
        path1RelativeTo2 = FileSystem::GetRelativePath(path1, path2);
        path2RelativeTo1 = FileSystem::GetRelativePath(path2, path1);
        SSASSERT(path1RelativeTo2 == ".");
        SSASSERT(path2RelativeTo1 == ".");
        SSASSERT(FileSystem::NormalizePath(path2 + '/' + path1RelativeTo2) == FileSystem::NormalizePath(path1));
        SSASSERT(FileSystem::NormalizePath(path1 + '/' + path2RelativeTo1) == FileSystem::NormalizePath(path2));
        SSASSERT(FileSystem::GetAbsolutePath(path2 + '/' + path1RelativeTo2) == FileSystem::GetAbsolutePath(path1));
        SSASSERT(FileSystem::GetAbsolutePath(path1 + '/' + path2RelativeTo1) == FileSystem::GetAbsolutePath(path2));

        path1 = "/f/g/h/i";
        path2 = "/f/g";
        path1RelativeTo2 = FileSystem::GetRelativePath(path1, path2);
        path2RelativeTo1 = FileSystem::GetRelativePath(path2, path1);
        SSASSERT(path1RelativeTo2 == "h/i");
        SSASSERT(path2RelativeTo1 == "../..");
        SSASSERT(FileSystem::NormalizePath(path2 + '/' + path1RelativeTo2) == FileSystem::NormalizePath(path1));
        SSASSERT(FileSystem::NormalizePath(path1 + '/' + path2RelativeTo1) == FileSystem::NormalizePath(path2));
        SSASSERT(FileSystem::GetAbsolutePath(path2 + '/' + path1RelativeTo2) == FileSystem::GetAbsolutePath(path1));
        SSASSERT(FileSystem::GetAbsolutePath(path1 + '/' + path2RelativeTo1) == FileSystem::GetAbsolutePath(path2));
    }

    {
        String path1 = "a/b/c/d/e";
        String path2 = "b/f/g";
        String path1RelativeTo2 = FileSystem::GetRelativePath(path1, path2);
        String path2RelativeTo1 = FileSystem::GetRelativePath(path2, path1);
        SSASSERT(path1RelativeTo2 == "../../../a/b/c/d/e");
        SSASSERT(path2RelativeTo1 == "../../../../../b/f/g");
        SSASSERT(FileSystem::NormalizePath(path2 + '/' + path1RelativeTo2) == FileSystem::NormalizePath(path1));
        SSASSERT(FileSystem::NormalizePath(path1 + '/' + path2RelativeTo1) == FileSystem::NormalizePath(path2));
        SSASSERT(FileSystem::GetAbsolutePath(path2 + '/' + path1RelativeTo2) == FileSystem::GetAbsolutePath(path1));
        SSASSERT(FileSystem::GetAbsolutePath(path1 + '/' + path2RelativeTo1) == FileSystem::GetAbsolutePath(path2));

        path1 = "a/b/../c/d/e";
        path2 = "b/f/../g";
        path1RelativeTo2 = FileSystem::GetRelativePath(path1, path2);
        path2RelativeTo1 = FileSystem::GetRelativePath(path2, path1);
        SSASSERT(path1RelativeTo2 == "../../a/c/d/e");
        SSASSERT(path2RelativeTo1 == "../../../../b/g");
        SSASSERT(FileSystem::NormalizePath(path2 + '/' + path1RelativeTo2) == FileSystem::NormalizePath(path1));
        SSASSERT(FileSystem::NormalizePath(path1 + '/' + path2RelativeTo1) == FileSystem::NormalizePath(path2));
        SSASSERT(FileSystem::GetAbsolutePath(path2 + '/' + path1RelativeTo2) == FileSystem::GetAbsolutePath(path1));
        SSASSERT(FileSystem::GetAbsolutePath(path1 + '/' + path2RelativeTo1) == FileSystem::GetAbsolutePath(path2));
    }
}

void test_file_related()
{
    using namespace ss;
    SSASSERT(FileSystem::IsFile(__FILE__));
    SSASSERT(!FileSystem::IsDirectory(__FILE__));
    SSASSERT(!FileSystem::IsFile(FileSystem::GetParent(__FILE__)));
    SSASSERT(FileSystem::IsDirectory(FileSystem::GetParent(__FILE__)));

    {
        String newDir = FileSystem::GetParent(__FILE__) + String("/notexist");
        String newFile = FileSystem::MakePath(newDir, "a_file");
        SSASSERT(FileSystem::Exists(__FILE__));
        SSASSERT(!FileSystem::Exists(newDir));
        SSASSERT(!FileSystem::Exists(newFile));

        SSASSERT(!FileSystem::MakeDir(__FILE__)); // name conflict
        SSASSERT(FileSystem::MakeDir(newDir));
        SSASSERT(FileSystem::Exists(newDir));
        SSASSERT(FileSystem::IsDirectory(newDir));

        SSASSERT(!FileSystem::Delete(newFile)); // delete a non-existing file
        SSASSERT(FileSystem::MakeFile(newFile));
        SSASSERT(FileSystem::Exists(newFile));
        SSASSERT(FileSystem::IsFile(newFile));

        SSASSERT(!FileSystem::Delete(newDir)); // delete a non-empty directory
        SSASSERT(FileSystem::Delete(newFile));
        SSASSERT(FileSystem::Delete(newDir)); // delete an empty directory
    }

    {
        String newDir = FileSystem::GetParent(__FILE__) + String("/test/mkdirs");
        String newFile = FileSystem::MakePath(newDir, "a_file");
        SSASSERT(FileSystem::MakeDirs(newDir));
        SSASSERT(FileSystem::MakeFile(newFile));
        SSASSERT(FileSystem::Exists(newDir));
        SSASSERT(FileSystem::IsDirectory(newDir));
        SSASSERT(FileSystem::Exists(newFile));
        SSASSERT(FileSystem::IsFile(newFile));

        String rootDir = FileSystem::GetParent(__FILE__) + "/test";
        SSASSERT(FileSystem::Delete(rootDir, true)); // delete a non-empty directory, recursively
    }
}

bool test()
{
    using namespace ss;
    String cwd = FileSystem::GetCWD();

    SSASSERT(FileSystem::IsAbsolutePath("/fdsaf/d/saf/ds/af/ds/f"));
    SSASSERT(FileSystem::IsAbsolutePath("\\//fdsaf/d/saf/ds/af/ds/f"));
    SSASSERT(!FileSystem::IsAbsolutePath("af/ds/af/ds/f"));
    SSASSERT(!FileSystem::IsAbsolutePath("./fdsaf/d/saf/ds/af/ds/f"));
    SSASSERT(!FileSystem::IsAbsolutePath(""));
    SSASSERT(FileSystem::IsAbsolutePath("/fdsaf/d/saf/ds/af/ds/"));

    SSASSERT("/fdsaf/d/saf/ds/af/ds/f" == FileSystem::NormalizePath("/fdsaf/d/saf/ds/af/ds/f"));
    SSASSERT("/fdsaf/d/saf/ds/af/ds/f" == FileSystem::NormalizePath("\\//fdsaf/d/saf/ds/af/ds/f"));
    SSASSERT("/fdsaf/ds/ds/f" == FileSystem::NormalizePath("/fdsaf/d/../ds/af/../ds/f"));
    SSASSERT("/" == FileSystem::NormalizePath("/fdsaf/d/../ds/af/../ds/f/../../../.."));
    SSASSERT("/.." == FileSystem::NormalizePath("/fdsaf/d/../ds/af/../ds/f/../../../../.."));
    SSASSERT("af/ds/af/ds/f" == FileSystem::NormalizePath("af/ds/af/ds/f"));
    SSASSERT("af/ds/af/ds/f" == FileSystem::NormalizePath("af/ds/af/ds/f"));
    SSASSERT("af/ds/af/ds/f" == FileSystem::NormalizePath("af/ds/af/ds/f/"));
    SSASSERT("af/ds/af/ds/f" == FileSystem::NormalizePath("af/ds/af/ds/f////"));
    SSASSERT("ds/f" == FileSystem::NormalizePath("af/ds/../../ds/f"));
    SSASSERT("ds/f" == FileSystem::NormalizePath("./af/ds/../../ds/f"));
    SSASSERT("../ds/f" == FileSystem::NormalizePath("../af/ds/../../ds/f"));
    SSASSERT("." == FileSystem::NormalizePath("af/ds/../../ds/f/../.."));
    SSASSERT(".." == FileSystem::NormalizePath("af/ds/../../ds/f/../../.."));
    SSASSERT("../ds/f" == FileSystem::NormalizePath("af/ds/../../../ds/f"));
    SSASSERT("../ds/f" == FileSystem::NormalizePath("af/ds/../.././././//../ds/f"));

    SSASSERT(cwd + "/a/b/c" == FileSystem::GetAbsolutePath("./a/b/c"));
    SSASSERT(cwd + "/a/b/c" == FileSystem::GetAbsolutePath("a/b/c"));
    SSASSERT(FileSystem::GetParent(cwd) == FileSystem::GetAbsolutePath("a/../a/b/c/../../../.."));

    SSASSERT(FileSystem::GetFileName("../..") == "..");
    SSASSERT(FileSystem::GetFileName("../../name") == "name");
    SSASSERT(FileSystem::GetFileExtName("../../name.txt.gz") == "gz");

    test_relative_path();

    test_file_related();

    return true;
}

} // namespace TestFileSystem
