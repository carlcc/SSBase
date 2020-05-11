//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Assert.h>
#include <SSBase/Str.h>
#include <set>

namespace TestString {

bool test()
{
    using namespace ss;
    String s1("hello，你好，world!");
    String s2(L"hello，你好，world!");
    String s3 = s1;
    StringView sv1 = s1.SubStringView(0, s1.Length());
    String s4(sv1);

    SSASSERT(s1 + s2 == "hello，你好，world!hello，你好，world!");
    SSASSERT(sv1 + s2 == "hello，你好，world!hello，你好，world!");
    SSASSERT(sv1 + "11111" == "hello，你好，world!11111");
    SSASSERT(s1.StartsWith("hello，"));
    SSASSERT(s1.EndsWith("，world!"));
    SSASSERT(s1.Contains("，你好"));
    SSASSERT(s1.Find("o，你好") == 4);
    SSASSERT(s1.Find(s1) == 0);
    SSASSERT(s1.RFind("o") == 10);
    SSASSERT(s1.RFind("o", 11) == 10);
    SSASSERT(s1.RFind("o", 10) == 4);
    SSASSERT(s1.RFind("o", 5) == 4);
    SSASSERT(s1.RFind("o", 4) == String::kNPos);
    SSASSERT(s1.RFind(s2) == 0);
    SSASSERT(s1.Compare("hello，你好，world!1") < 0);
    SSASSERT(s1.Compare("hello，你好，world!1") <= 0);
    SSASSERT(s1.Compare("hello，你好，world!") == 0);
    SSASSERT(s1.Compare("hello，你好，worlc!") != 0);
    SSASSERT(s1.Compare("hello，你好，worlc!") > 0);
    SSASSERT(s1.Compare("hello，你好，worlc!") >= 0);
    SSASSERT(s1.CompareIgnoreCase("hellp，你好，world!") != 0);
    SSASSERT(s1.CompareIgnoreCase("heLLO，你好，world!") == 0);
    SSASSERT(s1.CompareIgnoreCase("hellp，你好，world!") < 0);
    SSASSERT(s1.CompareIgnoreCase("hellP，你好，world!") <= 0);
    SSASSERT(s1.CompareIgnoreCase("hellA，你好，world!") > 0);
    SSASSERT(s1.CompareIgnoreCase("hella，你好，world!") >= 0);
    SSASSERT(s1 == s2);
    SSASSERT(s1 == s3);
    SSASSERT(s1 == s4);
    SSASSERT(s1 == sv1);
    SSASSERT(!(s1 != s2));
    SSASSERT(s1 >= s2);
    SSASSERT(s1 <= s2);
    SSASSERT(!(s1 > s2));
    SSASSERT(!(s1 < s2));
    s3 += String("12");
    SSASSERT(s1 < s3);
    SSASSERT(s3 > s1);
    SSASSERT(s1 <= s3);
    SSASSERT(s3 >= s1);
    SSASSERT(!(s1 == s3));
    SSASSERT(!(s3 == s1));
    SSASSERT(s1 != s3);
    SSASSERT(s3 != s1);
    s1 += "12";
    s2 += L"12";
    SSASSERT(s1 == s3);
    SSASSERT(s1 == s2);
    SSASSERT(!(s1 != s2));
    SSASSERT(s1 >= s2);
    SSASSERT(s1 <= s2);
    SSASSERT(!(s1 > s2));
    SSASSERT(!(s1 < s2));

    s1.Resize(s1.Length() - 2);
    s2 = s2.SubString(0, s2.Length() - 2);

    StringView sv2 = s3.SubStringView(0, s3.Length() - 2);
    SSASSERT(sv2 == s2);
    SSASSERT(sv2 == s4);
    SSASSERT(sv2 == sv1);

    String s5 = s3.SubString(0, s3.Length() - 2);
    SSASSERT(s5 == s1);

    s3 = s3.SubString(0, s3.Length() - 2);
    SSASSERT(s3 == s1);

    std::cout << "cout   s1: " << s1 << std::endl;
    std::cout << "cout   s2: " << s2 << std::endl;
    std::cout << "cout   s3: " << s3 << std::endl;
    std::cout << "cout   s4: " << s4 << std::endl;
    std::cout << "cout   s5: " << s5 << std::endl;
    std::cout << "cout  sv1: " << sv1 << std::endl;
    std::cout << "cout  sv2: " << sv2 << std::endl;

    sv2[2] = 'L';
    SSASSERT(sv2[2] == s3[2]);
    SSASSERT(sv2[2] != s1[2]);

    String s6 = "Ｈｅｌｌｏ,World，你好世界！";
    SSASSERT(s6.CopyToUpper() == "ＨＥＬＬＯ,WORLD，你好世界！");
    SSASSERT(s6.CopyToLower() == "ｈｅｌｌｏ,world，你好世界！");
    s6.ToUpper();
    SSASSERT(s6 == "ＨＥＬＬＯ,WORLD，你好世界！");
    s6.ToLower();
    SSASSERT(s6 == "ｈｅｌｌｏ,world，你好世界！");

    {
        String strToSplit = "Hello,world,nice,世界，!";
        std::vector<String> strs = strToSplit.Split(",");
        SSASSERT(strs.size() == 4);
        SSASSERT(strs[0] == "Hello");
        SSASSERT(strs[1] == "world");
        SSASSERT(strs[2] == "nice");
        SSASSERT(strs[3] == "世界，!");

        strToSplit = "Hello,,world,,nice,,世界，!";
        std::set<String> strset = strToSplit.Split(",,");
        SSASSERT(strset.size() == 4);
        String result[4] = { "Hello", "nice", "world", "世界，!" };
        uint32_t i = 0;
        for (auto& s : strset) {
            SSASSERT(s == result[i++]);
        }

        auto newStr = strToSplit.ReplaceAll(",,", "OO");
        SSASSERT(newStr == "HelloOOworldOOniceOO世界，!");
        std::cout << newStr << std::endl;
        newStr = newStr.ReplaceAll("oO", "---");
        std::cout << newStr << std::endl;
        SSASSERT(newStr == "Hell---OworldOOniceOO世界，!");
    }

    {
        String needTrim1 = " \t\n\r\vhello   \tworld\t\r\n\v  ";
        String needTrim2 = " \t\n\r\vhello   \tworld\t\r\n\v  ";
        String needTrim3 = " \t\n\r\vhello   \tworld\t\r\n\v  ";
        needTrim1.TrimLeading();
        needTrim2.TrimTrailing();
        needTrim3.Trim();
        SSASSERT(needTrim1 == "hello   \tworld\t\r\n\v  ");
        SSASSERT(needTrim2 == " \t\n\r\vhello   \tworld");
        SSASSERT(needTrim3 == "hello   \tworld");
    }
    {
        String placeHolder("English:'{},{}'");
        String placeHolder2("Chinese:'{},{}'");
        StringView placeHolderSv = placeHolder2.SubStringView(0);
        String fmt = "{}!,{}!";
        String result = fmt.Format(placeHolder, placeHolderSv).Format("Hello", "world", "你好", "世界");
        SSASSERT(result == "English:'Hello,world'!,Chinese:'你好,世界'!");

        fmt = "PI is '{:.2f}'";
        result = fmt.Format(3.14159);
        SSASSERT(result == "PI is '3.14'");
    }

    return true;
}

}