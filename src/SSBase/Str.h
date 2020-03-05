//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include "internal/Str.h"
#include "thirdparty/fmt/fmt/format.h"

namespace ss
{

class String;
class StringView;

/// CharSequence object should be a derived class, pure CharSequence object is not allowd.
class CharSequence
{
public:
    using CharType = uint32_t;
    enum
    {
        kNPos = uint32_t(-1)
    };
    enum CharSet
    {
        kUtf8,
        kLocale = kUtf8
    };

    static bool IsAsciiAlpha(CharType c);
    static bool IsAlpha(CharType c);
    static bool IsUpper(CharType c);
    static bool IsLower(CharType c);
    static CharType ToUpper(CharType c);
    static CharType ToLower(CharType c);
    /// From MSDN(https://docs.microsoft.com/en-us/dotnet/api/system.char.iswhitespace?view=netframework-4.8)
    /// White space characters are the following Unicode characters:
    ///
    /// Members of the UnicodeCategory.SpaceSeparator category, which includes the characters SPACE (U+0020), NO-BREAK
    /// SPACE (U+00A0), OGHAM SPACE MARK (U+1680), EN QUAD (U+2000), EM QUAD (U+2001), EN SPACE (U+2002), EM SPACE
    /// (U+2003), THREE-PER-EM SPACE (U+2004), FOUR-PER-EM SPACE (U+2005), SIX-PER-EM SPACE (U+2006), FIGURE SPACE
    /// (U+2007), PUNCTUATION SPACE (U+2008), THIN SPACE (U+2009), HAIR SPACE (U+200A), NARROW NO-BREAK SPACE (U+202F),
    /// MEDIUM MATHEMATICAL SPACE (U+205F), and IDEOGRAPHIC SPACE (U+3000).
    ///
    /// Members of the UnicodeCategory.LineSeparator category, which consists solely of the LINE SEPARATOR character
    /// (U+2028).
    ///
    /// Members of the UnicodeCategory.ParagraphSeparator category, which consists solely of the PARAGRAPH SEPARATOR
    /// character (U+2029).
    ///
    /// The characters CHARACTER TABULATION (U+0009), LINE FEED (U+000A), LINE TABULATION (U+000B), FORM FEED (U+000C),
    /// CARRIAGE RETURN (U+000D), and NEXT LINE (U+0085).
    static bool IsWhiteSpace(CharType c);

protected:
    struct SequenceData;

    CharSequence();

public:
    CharSequence(const CharSequence &) = delete;
    CharSequence(CharSequence &&) = delete;

    virtual ~CharSequence() = default;

    String SubString(uint32_t from, uint32_t length = kNPos) const;

    StringView SubStringView(uint32_t from, uint32_t length = kNPos) const;

    CharType operator[](uint32_t index) const
    {
        return At(index);
    }

    CharType &operator[](uint32_t index)
    {
        return At(index);
    }

    CharType At(uint32_t index) const
    {
        return Data()[index];
    }

    CharType &At(uint32_t index)
    {
        return Data()[index];
    }

    virtual CharType *Data() = 0;

    virtual const CharType *Data() const = 0;

    uint32_t Length() const
    {
        return length_;
    }

    bool Empty() const
    {
        return Length() == 0;
    }

    int Compare(const CharSequence &s) const;
    int Compare(const String &s) const
    {
        return Compare((const CharSequence &)s);
    }
    int CompareIgnoreCase(const CharSequence &s) const;
    int CompareIgnoreCase(const String &s) const
    {
        return CompareIgnoreCase((const CharSequence &)s);
    }

    bool StartsWith(const CharSequence &s) const;
    bool StartsWith(const String &s) const;

    bool EndsWith(const CharSequence &s) const;
    bool EndsWith(const String &s) const;

    bool Contains(const CharSequence &s) const
    {
        return Find(s) != kNPos;
    }
    bool Contains(const String &s) const
    {
        return Find(s) != kNPos;
    }

    uint32_t Find(const CharSequence &s, uint32_t start = 0) const;
    uint32_t Find(const String &s, uint32_t start = 0) const
    {
        return Find((const CharSequence &)s, start);
    }
    /// NOTE: end is not included
    uint32_t RFind(const CharSequence &s, uint32_t end = kNPos) const;
    uint32_t RFind(const String &s, uint32_t end = kNPos) const
    {
        return RFind((const CharSequence &)s, end);
    }

    void ToUpper();
    void ToLower();
    String CopyToUpper() const;
    String CopyToLower() const;

    StrSplitter Split(const CharSequence &splitter) const
    {
        return {*this, splitter};
    }

    StrSplitter Split(const String &splitter) const
    {
        return Split((const CharSequence &)splitter);
    }

    /// insert the split-ed strings to container `c`, you may need to clear it before invoking this method.
    template <class Container> void Split(const CharSequence &splitter, Container &c) const
    {
        StrSplitter(*this, splitter).Split(c);
    }

    String ReplaceAll(const CharSequence &oldStr, const CharSequence &newStr);
    String ReplaceAll(const String &oldStr, const CharSequence &newStr);
    String ReplaceAll(const CharSequence &oldStr, const String &newStr);
    String ReplaceAll(const String &oldStr, const String &newStr);

    uint32_t GetBytesLength(CharSet charSet) const;
    /// NOTE: This function assumes the buffer is big enough, you can check the required size by invoking
    /// `GetBytesLength` function.
    void GetBytes(CharSet charSet, void *buffer) const;
    std::string ToStdString(CharSet charSet = kLocale) const;

    uint64_t Hash() const;

protected:
    virtual StringView SubStringViewImpl(uint32_t from, uint32_t length) const = 0;

protected:
    SequenceData *sequenceData_;
    uint32_t length_;
};

class StringView : public CharSequence
{
public:
    StringView(SequenceData *sequenceData, uint32_t offset, uint32_t length);
    StringView(const StringView &sv);
    StringView(StringView &&sv) noexcept;
    ~StringView() override;

    CharType *Data() override;
    const CharType *Data() const override;

    StringView &operator=(const StringView &sv);
    StringView &operator=(StringView &&sv) noexcept;

protected:
    StringView SubStringViewImpl(uint32_t from, uint32_t length) const override;

private:
    uint32_t offset_;
};

class String : public CharSequence
{
public:
    explicit String();
    String(char c);
    String(wchar_t c);
    String(const char *utf8);       // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    String(const wchar_t *unicode); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    String(const CharType *chars, uint32_t length);
    String(const CharSequence &s); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    String(const String &s);
    String(String &&s) noexcept;
    ~String() override;

    CharType *Data() override;
    const CharType *Data() const override;
    String &operator=(const String &s);
    String &operator=(String &&s) noexcept;
    String &operator+=(const String &s);
    String &operator=(const CharSequence &s);
    String &operator+=(const CharSequence &s);
    String &operator=(const char *utf8);
    String &operator+=(const char *utf8);
    String &operator=(const wchar_t *unicode);
    String &operator+=(const wchar_t *unicode);
    String &operator+=(const wchar_t unicode);
    String &operator+=(const char unicode);

    uint32_t Capacity() const
    {
        return capacity_;
    }

    void Reserve(uint32_t n);
    void Resize(uint32_t n);

    void Trim()
    {
        TrimTrailing();
        TrimLeading();
    }
    void TrimLeading();
    void TrimTrailing();

    /// API refer to: https://fmt.dev/dev/api.html
    /// FIXME: A better solution?
    template <class... Args> String Format(Args &&... args)
    {
        auto utf8Len = GetBytesLength(kUtf8);
        std::string utf8(utf8Len, '\0');
        GetBytes(kUtf8, &utf8[0]);
        return String(fmt::format(utf8, std::forward<Args>(args)...).c_str());
    }

private:
    StringView SubStringViewImpl(uint32_t from, uint32_t length) const override;
    void ReAllocate(uint32_t newCapacity);
    void Free();

private:
    uint32_t capacity_;
};

inline String operator+(const CharSequence &s1, const CharSequence &s2)
{
    String s(s1);
    s += s2;
    return s;
}

inline String operator+(const CharSequence &s1, const String &s2)
{
    String s(s1);
    s += s2;
    return s;
}

inline String operator+(const CharSequence &s1, char c)
{
    String s(s1);
    s += c;
    return s;
}

inline String operator+(char c, const CharSequence &s1)
{
    String s;
    s += c;
    s += s1;
    return s;
}

inline String operator+(const CharSequence &s1, wchar_t c)
{
    String s(s1);
    s += c;
    return s;
}

inline String operator+(wchar_t c, const CharSequence &s1)
{
    String s;
    s += c;
    s += s1;
    return s;
}

inline String operator+(const String &s1, const CharSequence &s2)
{
    String s(s1);
    s += s2;
    return s;
}

inline String operator+(const String &s1, const String &s2)
{
    String s(s1);
    s += s2;
    return s;
}

inline String operator+(const String &s1, char c)
{
    String s(s1);
    s += c;
    return s;
}

inline String operator+(char c, const String &s1)
{
    String s;
    s += c;
    s += s1;
    return s;
}

inline String operator+(const String &s1, wchar_t c)
{
    String s(s1);
    s += c;
    return s;
}

inline String operator+(wchar_t c, const String &s1)
{
    String s;
    s += c;
    s += s1;
    return s;
}

#define SS_DEFINE_COMPARE_OPERATOR(type1, type2)                                                                       \
    inline bool operator==(const type1 &s1, const type2 &s2)                                                           \
    {                                                                                                                  \
        return s1.Compare(s2) == 0;                                                                                    \
    }                                                                                                                  \
    inline bool operator!=(const type1 &s1, const type2 &s2)                                                           \
    {                                                                                                                  \
        return s1.Compare(s2) != 0;                                                                                    \
    }                                                                                                                  \
    inline bool operator>(const type1 &s1, const type2 &s2)                                                            \
    {                                                                                                                  \
        return s1.Compare(s2) > 0;                                                                                     \
    }                                                                                                                  \
    inline bool operator<(const type1 &s1, const type2 &s2)                                                            \
    {                                                                                                                  \
        return s1.Compare(s2) < 0;                                                                                     \
    }                                                                                                                  \
    inline bool operator>=(const type1 &s1, const type2 &s2)                                                           \
    {                                                                                                                  \
        return s1.Compare(s2) >= 0;                                                                                    \
    }                                                                                                                  \
    inline bool operator<=(const type1 &s1, const type2 &s2)                                                           \
    {                                                                                                                  \
        return s1.Compare(s2) <= 0;                                                                                    \
    }

SS_DEFINE_COMPARE_OPERATOR(CharSequence, CharSequence)
SS_DEFINE_COMPARE_OPERATOR(CharSequence, String)
SS_DEFINE_COMPARE_OPERATOR(String, CharSequence)
SS_DEFINE_COMPARE_OPERATOR(String, String)

#undef SS_DEFINE_COMPARE_OPERATOR

std::ostream &operator<<(std::ostream &os, const CharSequence &s);

} // namespace ss

#define SS_DEFINE_FORMATTER(type)                                                                                      \
    template <> struct fmt::formatter<type>                                                                            \
    {                                                                                                                  \
        constexpr auto parse(format_parse_context &ctx)                                                                \
        {                                                                                                              \
            return ctx.begin();                                                                                        \
        }                                                                                                              \
                                                                                                                       \
        template <typename FormatContext> auto format(const type &s, FormatContext &ctx)                               \
        {                                                                                                              \
            auto utf8Len = s.GetBytesLength(ss::CharSequence::kUtf8);                                                  \
            std::string bytes(utf8Len, '\0');                                                                          \
            s.GetBytes(ss::CharSequence::kUtf8, &bytes[0]);                                                            \
            return format_to(ctx.out(), "{}", bytes);                                                                  \
        }                                                                                                              \
    };

SS_DEFINE_FORMATTER(ss::CharSequence)
SS_DEFINE_FORMATTER(ss::String)
SS_DEFINE_FORMATTER(ss::StringView)

#undef SS_DEFINE_FORMATTER

#include "internal/Str.inl"