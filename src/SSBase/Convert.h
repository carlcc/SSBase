//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Str.h"
#include <sstream>
#include <string>

namespace ss {

class Convert {
public:
    template <class T>
    static T StringTo(const String& s)
    {
        std::stringstream ss;
        ss << s;
        T t;
        ss >> t;
        return t;
    }

    template <class T>
    static T StringTo(const char* s)
    {
        std::stringstream ss;
        ss << s;
        T t;
        ss >> t;
        return t;
    }

    template <class T>
    static T StringTo(const std::string& s)
    {
        std::stringstream ss;
        ss << s;
        T t;
        ss >> t;
        return t;
    }

    template <class T>
    static T StringTo(const std::wstring& s)
    {
        std::wstringstream ss;
        ss << s;
        T t;
        ss >> t;
        return t;
    }

    template <class T>
    static T StringTo(const wchar_t* s)
    {
        std::wstringstream ss;
        ss << s;
        T t;
        ss >> t;
        return t;
    }
};

} // namespace ss