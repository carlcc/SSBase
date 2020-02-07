//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#include "Str.h"
#include "Assert.h"
#include "Misc.h"
#include "thirdparty/fmt/fmt/format.h"
#include <cstring>
#include <cwchar>
#include <unordered_set>

namespace ss
{

// modified version of utfcpp : https://github.com/ww898/utf-cpp
// Supported combinations:
//   0xxx_xxxx
//   110x_xxxx 10xx_xxxx
//   1110_xxxx 10xx_xxxx 10xx_xxxx
//   1111_0xxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
//   1111_10xx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
//   1111_110x 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
struct utf8 final
{
    static size_t const max_unicode_symbol_size = 4;
    static size_t const max_supported_symbol_size = 6;

    static uint32_t const max_supported_code_point = 0x7FFFFFFF;

    using char_type = uint8_t;

    template <typename PeekFn> static size_t char_size(PeekFn &&peek_fn)
    {
        char_type const ch0 = std::forward<PeekFn>(peek_fn)();
        if (ch0 < 0x80) // 0xxx_xxxx
            return 1;
        if (ch0 < 0xC0)
            // throw std::runtime_error("The utf8 first char in sequence is incorrect");
            return -1;
        if (ch0 < 0xE0) // 110x_xxxx 10xx_xxxx
            return 2;
        if (ch0 < 0xF0) // 1110_xxxx 10xx_xxxx 10xx_xxxx
            return 3;
        if (ch0 < 0xF8) // 1111_0xxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
            return 4;
        if (ch0 < 0xFC) // 1111_10xx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
            return 5;
        if (ch0 < 0xFE) // 1111_110x 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
            return 6;
        return -1;
        // throw std::runtime_error("The utf8 first char in sequence is incorrect");
    }

    template <typename ReadFn> static uint32_t read(ReadFn &&read_fn)
    {
        char_type const ch0 = read_fn();
        if (ch0 < 0x80) // 0xxx_xxxx
            return ch0;
        if (ch0 < 0xC0)
            // throw std::runtime_error("The utf8 first char in sequence is incorrect");
            return -1;
        if (ch0 < 0xE0) // 110x_xxxx 10xx_xxxx
        {
            char_type const ch1 = read_fn();
            if (ch1 >> 6 != 2)
                goto _err;
            return (ch0 << 6) + ch1 - 0x3080;
        }
        if (ch0 < 0xF0) // 1110_xxxx 10xx_xxxx 10xx_xxxx
        {
            char_type const ch1 = read_fn();
            if (ch1 >> 6 != 2)
                goto _err;
            char_type const ch2 = read_fn();
            if (ch2 >> 6 != 2)
                goto _err;
            return (ch0 << 12) + (ch1 << 6) + ch2 - 0xE2080;
        }
        if (ch0 < 0xF8) // 1111_0xxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            char_type const ch1 = read_fn();
            if (ch1 >> 6 != 2)
                goto _err;
            char_type const ch2 = read_fn();
            if (ch2 >> 6 != 2)
                goto _err;
            char_type const ch3 = read_fn();
            if (ch3 >> 6 != 2)
                goto _err;
            return (ch0 << 18) + (ch1 << 12) + (ch2 << 6) + ch3 - 0x3C82080;
        }
        if (ch0 < 0xFC) // 1111_10xx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            char_type const ch1 = read_fn();
            if (ch1 >> 6 != 2)
                goto _err;
            char_type const ch2 = read_fn();
            if (ch2 >> 6 != 2)
                goto _err;
            char_type const ch3 = read_fn();
            if (ch3 >> 6 != 2)
                goto _err;
            char_type const ch4 = read_fn();
            if (ch4 >> 6 != 2)
                goto _err;
            return (ch0 << 24) + (ch1 << 18) + (ch2 << 12) + (ch3 << 6) + ch4 - 0xFA082080;
        }
        if (ch0 < 0xFE) // 1111_110x 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            char_type const ch1 = read_fn();
            if (ch1 >> 6 != 2)
                goto _err;
            char_type const ch2 = read_fn();
            if (ch2 >> 6 != 2)
                goto _err;
            char_type const ch3 = read_fn();
            if (ch3 >> 6 != 2)
                goto _err;
            char_type const ch4 = read_fn();
            if (ch4 >> 6 != 2)
                goto _err;
            char_type const ch5 = read_fn();
            if (ch5 >> 6 != 2)
                goto _err;
            return (ch0 << 30) + (ch1 << 24) + (ch2 << 18) + (ch3 << 12) + (ch4 << 6) + ch5 - 0x82082080;
        }
        throw std::runtime_error("The utf8 first char in sequence is incorrect");
    _err:
        throw std::runtime_error("The utf8 slave char in sequence is incorrect");
    }

    template <typename WriteFn> static void write(uint32_t const cp, WriteFn &&write_fn)
    {
        if (cp < 0x80) // 0xxx_xxxx
            write_fn(static_cast<char_type>(cp));
        else if (cp < 0x800) // 110x_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xC0 | cp >> 6));
            goto _1;
        }
        else if (cp < 0x10000) // 1110_xxxx 10xx_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xE0 | cp >> 12));
            goto _2;
        }
        else if (cp < 0x200000) // 1111_0xxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xF0 | cp >> 18));
            goto _3;
        }
        else if (cp < 0x4000000) // 1111_10xx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xF8 | cp >> 24));
            goto _4;
        }
        else if (cp < 0x80000000) // 1111_110x 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xFC | cp >> 30));
            goto _5;
        }
        else
            throw std::runtime_error("Tool large UTF8 code point");
        return;
    _5:
        write_fn(static_cast<char_type>(0x80 | (cp >> 24 & 0x3F)));
    _4:
        write_fn(static_cast<char_type>(0x80 | (cp >> 18 & 0x3F)));
    _3:
        write_fn(static_cast<char_type>(0x80 | (cp >> 12 & 0x3F)));
    _2:
        write_fn(static_cast<char_type>(0x80 | (cp >> 6 & 0x3F)));
    _1:
        write_fn(static_cast<char_type>(0x80 | (cp & 0x3F)));
    }
};

struct CharSequence::SequenceData
{
    CharType *chars_;

    explicit SequenceData(uint32_t length)
    {
        chars_ = static_cast<CharType *>(malloc(length * sizeof(CharType)));
    }

    ~SequenceData()
    {
        free(chars_);
    }

    void ReAllocate(uint32_t length)
    {
        chars_ = static_cast<CharType *>(realloc(chars_, length * sizeof(CharType)));
    }

    static SequenceData *New(uint32_t length)
    {
        return new SequenceData(length);
    }

    static void Delete(SequenceData *sd)
    {
        delete sd;
    }
};

bool CharSequence::IsUpper(CharSequence::CharType c)
{
    const uint32_t A = 'A';
    const uint32_t Z = 'Z';
    const uint32_t AA = L'Ａ';
    const uint32_t ZZ = L'Ｚ';
    return (c >= A && c <= Z) || (c >= AA && c <= ZZ);
}

bool CharSequence::IsLower(CharSequence::CharType c)
{
    const uint32_t a = 'a';
    const uint32_t z = 'z';
    const uint32_t aa = L'ａ';
    const uint32_t zz = L'ｚ';
    return (c >= aa && c <= zz) || (c >= a && c <= z);
}

CharSequence::CharType CharSequence::ToUpper(CharSequence::CharType c)
{
    const uint32_t a = 'a';
    const uint32_t A = 'A';
    const uint32_t z = 'z';
    const uint32_t aa = L'ａ';
    const uint32_t AA = L'Ａ';
    const uint32_t zz = L'ｚ';
    if (c >= a && c <= z)
    {
        return c + A - a;
    }
    if (c >= aa && c <= zz)
    {
        return c + AA - aa;
    }
    return c;
}

CharSequence::CharType CharSequence::ToLower(CharSequence::CharType c)
{
    const uint32_t a = 'a';
    const uint32_t A = 'A';
    const uint32_t Z = 'Z';
    const uint32_t aa = L'ａ';
    const uint32_t AA = L'Ａ';
    const uint32_t ZZ = L'Ｚ';
    if (c >= A && c <= Z)
    {
        return c + a - A;
    }
    if (c >= AA && c <= ZZ)
    {
        return c + aa - AA;
    }
    return c;
}

bool CharSequence::IsWhiteSpace(CharSequence::CharType c)
{
    enum Spaces : uint32_t
    {
        ///
        TABULATION = 0x0009,
        LINE_FEED = 0x000A,
        LINE_TABULATION = 0x000B,
        FORM_FEED = 0x000C,
        CARRIAGE_RETURN = 0x000D,
        ///
        SPACE = 0x20,
        NO_BREAK_SPACE = 0xA0,
        OGHAM_SPACE_MARK = 0x1680,
        ///
        EN_QUAD = 0x2000,
        EM_QUAD = 0x2001,
        EN_SPACE = 0x2002,
        EM_SPACE = 0x2003,
        THREE_PER_EM_SPACE = 0x2004,
        FOUR_PER_EM_SPACE = 0x2005,
        SIX_PER_EM_SPACE = 0x2006,
        FIGURE_SPACE = 0x2007,
        PUNCTUATION_SPACE = 0x2008,
        THIN_SPACE = 0x2009,
        HAIR_SPACE = 0x200A,
        ///
        NARROW_NO_BREAK_SPACE = 0x202F,
        MEDIUM_MATHEMATICAL_SPACE = 0x205F,
        IDEOGRAPHIC_SPACE = 0x3000,
        ///
        LINE_SEPARATOR = 0x2028,
        PARAGRAPH_SEPARATOR = 0x2029,
    }; ///
    static const std::unordered_set<uint32_t> kSpaces = {
        TABULATION,
        LINE_FEED,
        LINE_TABULATION,
        FORM_FEED,
        CARRIAGE_RETURN,
        SPACE,
        NO_BREAK_SPACE,
        OGHAM_SPACE_MARK,
        EN_QUAD,
        EM_QUAD,
        EN_SPACE,
        EM_SPACE,
        THREE_PER_EM_SPACE,
        FOUR_PER_EM_SPACE,
        SIX_PER_EM_SPACE,
        FIGURE_SPACE,
        PUNCTUATION_SPACE,
        THIN_SPACE,
        HAIR_SPACE,
        NARROW_NO_BREAK_SPACE,
        MEDIUM_MATHEMATICAL_SPACE,
        IDEOGRAPHIC_SPACE,
        LINE_SEPARATOR,
        PARAGRAPH_SEPARATOR,
    };
    return kSpaces.find(c) != kSpaces.end();
}

CharSequence::CharSequence() : sequenceData_(nullptr), length_(0)
{
}

String CharSequence::SubString(uint32_t from, uint32_t length) const
{
    if (from >= Length())
    {
        return String();
    }
    if (uint64_t(from) + length > Length()) // from + length may larger than 2^32-1
    {
        length = Length() - from;
    }
    return String(Data() + from, length);
}

StringView CharSequence::SubStringView(uint32_t from, uint32_t length) const
{
    return SubStringViewImpl(from, length);
}

int CharSequence::Compare(const CharSequence &s) const
{
    uint32_t n = Length() > s.Length() ? s.Length() : Length();
    for (uint32_t i = 0; i < n; ++i)
    {
        if (At(i) > s.At(i))
        {
            return 1;
        }
        if (At(i) < s.At(i))
        {
            return -1;
        }
    }
    if (Length() > s.Length())
    {
        return 1;
    }
    if (Length() < s.Length())
    {
        return -1;
    }
    return 0;
}

int CharSequence::CompareIgnoreCase(const CharSequence &s) const
{
    String thisStr(*this);
    String otherStr(s);
    thisStr.ToLower();
    otherStr.ToLower();
    return thisStr.Compare(otherStr);
}

bool CharSequence::StartsWith(const CharSequence &s) const
{
    return Length() >= s.Length() && memcmp(Data(), s.Data(), s.Length() * sizeof(CharType)) == 0;
}

bool CharSequence::StartsWith(const String &s) const
{
    return StartsWith((const CharSequence &)s);
}

bool CharSequence::EndsWith(const CharSequence &s) const
{
    return Length() >= s.Length() &&
           memcmp(Data() + Length() - s.Length(), s.Data(), s.Length() * sizeof(CharType)) == 0;
}

bool CharSequence::EndsWith(const String &s) const
{
    return EndsWith((const CharSequence &)s);
}

uint32_t CharSequence::Find(const CharSequence &s, uint32_t start) const
{
    if (Length() < s.Length() + start)
    {
        return kNPos;
    }
    uint32_t n = Length() - s.Length() + 1;
    for (uint32_t i = start; i < n; ++i)
    {
        if (memcmp(Data() + i, s.Data(), s.Length() * sizeof(CharType)) == 0)
        {
            return i;
        }
    }
    return kNPos;
}

uint32_t CharSequence::RFind(const CharSequence &s, uint32_t end) const
{
    if (end > Length())
    {
        end = Length();
    }
    if (end < s.Length())
    {
        return kNPos;
    }

    uint32_t n = Length() - s.Length() + 1;
    for (uint32_t i = n; i > 0; --i)
    {
        if (memcmp(Data() + i - 1, s.Data(), s.Length() * sizeof(CharType)) == 0)
        {
            return i - 1;
        }
    }
    return kNPos;
}

void CharSequence::ToUpper()
{
    for (uint32_t i = 0; i < Length(); ++i)
    {
        At(i) = ToUpper(At(i));
    }
}

void CharSequence::ToLower()
{
    for (uint32_t i = 0; i < Length(); ++i)
    {
        At(i) = ToLower(At(i));
    }
}

String CharSequence::CopyToUpper() const
{
    String copy(*this);
    copy.ToUpper();
    return copy;
}

String CharSequence::CopyToLower() const
{
    String copy(*this);
    copy.ToLower();
    return copy;
}

String CharSequence::ReplaceAll(const CharSequence &oldStr, const CharSequence &newStr)
{
    std::vector<StringView> split = Split(oldStr);
    String result(split[0]);
    if (split.size() > 1)
    {
        uint32_t length = (split.size() - 1) * newStr.Length();
        for (auto &sv : split)
        {
            length += sv.Length();
        }

        result.Reserve(length);
        for (size_t i = 1; i < split.size(); ++i)
        {
            result += newStr;
            result += split[i];
        }
    }

    return result;
}

String CharSequence::ReplaceAll(const String &oldStr, const CharSequence &newStr)
{
    return ReplaceAll((const CharSequence &)oldStr, (const CharSequence &)newStr);
}

String CharSequence::ReplaceAll(const CharSequence &oldStr, const String &newStr)
{
    return ReplaceAll((const CharSequence &)oldStr, (const CharSequence &)newStr);
}

String CharSequence::ReplaceAll(const String &oldStr, const String &newStr)
{
    return ReplaceAll((const CharSequence &)oldStr, (const CharSequence &)newStr);
}

uint32_t CharSequence::GetBytesLength(CharSequence::CharSet charSet) const
{
    SSASSERT(charSet == kUtf8);
    uint32_t utf8Length = 0;
    for (uint32_t i = 0; i < Length(); ++i)
    {
        utf8::write(At(i), [&utf8Length](utf8::char_type c) { ++utf8Length; });
    }
    return utf8Length;
}

void CharSequence::GetBytes(CharSet charSet, void *buffer) const
{
    SSASSERT(charSet == kUtf8);
    auto *p = static_cast<utf8::char_type *>(buffer);
    uint32_t index = 0;
    for (uint32_t i = 0; i < Length(); ++i)
    {
        utf8::write(At(i), [p, &index](utf8::char_type c) { p[index++] = c; });
    }
    p[index] = '\0';
}

std::string CharSequence::ToStdString(CharSequence::CharSet charSet) const
{
    uint32_t utf8Length = GetBytesLength(charSet);

    std::string ss(utf8Length, L'\0');
    GetBytes(CharSequence::kUtf8, const_cast<char *>(ss.data()));
    return ss;
}

uint64_t CharSequence::Hash() const
{
    // djb2
    uint64_t hash = 5381;
    for (uint32_t i = 0; i < Length(); ++i)
    {
        hash = ((hash << 5u) + hash) + At(i); /* hash * 33 + c */
    }
    return hash;
}

StringView::StringView(SequenceData *sequenceData, uint32_t offset, uint32_t length) : CharSequence()
{
    sequenceData_ = sequenceData;
    offset_ = offset;
    length_ = length;
}

StringView::StringView(const StringView &sv) : CharSequence()
{
    sequenceData_ = sv.sequenceData_;
    offset_ = sv.offset_;
    length_ = sv.length_;
}

StringView::StringView(StringView &&sv) noexcept : CharSequence()
{
    sequenceData_ = sv.sequenceData_;
    offset_ = sv.offset_;
    length_ = sv.length_;
    sv.sequenceData_ = nullptr;
    sv.offset_ = 0;
    sv.length_ = 0;
}

StringView::~StringView()
{
    sequenceData_ = nullptr;
    offset_ = 0;
    length_ = 0;
}

StringView StringView::SubStringViewImpl(uint32_t from, uint32_t length) const
{
    if (from > length_) // start out of bounds
    {
        return StringView(sequenceData_, offset_ + length_, 0);
    }
    if (from + length > length_) // end out of bound
    {
        length = length_ - from;
    }
    from += offset_;
    return StringView(sequenceData_, from, length);
}

CharSequence::CharType *StringView::Data()
{
    return sequenceData_->chars_ + offset_;
}

const CharSequence::CharType *StringView::Data() const
{
    return sequenceData_->chars_ + offset_;
}

StringView &StringView::operator=(const StringView &sv)
{
    sequenceData_ = sv.sequenceData_;
    offset_ = sv.offset_;
    length_ = sv.length_;
    return *this;
}

StringView &StringView::operator=(StringView &&sv) noexcept
{
    sequenceData_ = sv.sequenceData_;
    offset_ = sv.offset_;
    length_ = sv.length_;
    sv.sequenceData_ = nullptr;
    sv.offset_ = 0;
    sv.length_ = 0;
    return *this;
}

String::String() : CharSequence(), capacity_(0)
{
    ReAllocate(16);
}

inline uint32_t GetUtf8Length(const char *utf8)
{
    uint32_t count = 0;
    uint32_t index = 0;

    auto fn = [&index, utf8]() -> utf8::char_type { return (utf8::char_type)utf8[index]; };
    while (utf8[index] != 0)
    {
        auto size = utf8::char_size(fn);
        if (size == -1)
        {
            return count;
        }
        ++count;
        index += size;
    }
    return count;
}

inline uint32_t CalculateCapacity(uint32_t len)
{
    return Misc::CeilToPowerOfTwo(len);
}

String::String(const char *utf8) : CharSequence(), capacity_(0)
{
    length_ = GetUtf8Length(utf8);

    ReAllocate(CalculateCapacity(length_));

    uint32_t index = 0;
    auto readFn = [&index, utf8]() -> utf8::char_type { return (utf8::char_type)utf8[index++]; };

    for (uint32_t i = 0; i < length_; ++i)
    {
        sequenceData_->chars_[i] = utf8::read(readFn);
    }
}

String::String(const wchar_t *unicode) : CharSequence(), capacity_(0)
{
    length_ = wcslen(unicode);
    ReAllocate(CalculateCapacity(length_));

    for (uint32_t i = 0; i < length_; ++i)
    {
        sequenceData_->chars_[i] = unicode[i];
    }
}

String::String(const CharSequence::CharType *chars, uint32_t length) : CharSequence(), capacity_(0)
{
    length_ = length;
    ReAllocate(CalculateCapacity(length_));

    for (uint32_t i = 0; i < length; ++i)
    {
        sequenceData_->chars_[i] = chars[i];
    }
}

String::String(const CharSequence &s) : CharSequence(), capacity_(0)
{
    length_ = s.Length();
    ReAllocate(CalculateCapacity(length_));
    memcpy(sequenceData_->chars_, s.Data(), length_ * sizeof(CharType));
}

String::String(const String &s) : CharSequence(), capacity_(0)
{
    new (this) String((CharSequence &)s);
}

String::String(String &&s) noexcept : CharSequence()
{
    length_ = s.length_;
    capacity_ = s.capacity_;
    sequenceData_ = s.sequenceData_;
    s.length_ = 0;
    s.capacity_ = 0;
    s.sequenceData_ = nullptr;
}

String::~String()
{
    Free();
}

CharSequence::CharType *String::Data()
{
    return sequenceData_->chars_;
}

const CharSequence::CharType *String::Data() const
{
    return sequenceData_->chars_;
}

StringView String::SubStringViewImpl(uint32_t from, uint32_t length) const
{
    if (from > length_) // Start out of bounds
    {
        return StringView(sequenceData_, length_, 0);
    }
    if (uint64_t(from) + length > length_) // from + length may larger than 2^32-1
    {
        length = length_ - from;
    }
    return StringView(sequenceData_, from, length);
}

String &String::operator=(const String &s)
{
    operator=((const CharSequence &)s);
    return *this;
}

String &String::operator+=(const String &s)
{
    operator+=((const CharSequence &)s);
    return *this;
}

String &String::operator=(String &&s) noexcept
{
    auto oldCap = capacity_;
    auto oldChars = sequenceData_->chars_;
    length_ = s.length_;
    capacity_ = s.capacity_;
    s.length_ = 0;
    s.capacity_ = oldCap;

    sequenceData_->chars_ = s.sequenceData_->chars_;
    s.sequenceData_->chars_ = oldChars;
    return *this;
}

String &String::operator=(const CharSequence &s)
{
    if (Capacity() < s.Length())
    {
        ReAllocate(CalculateCapacity(s.Length()));
    }
    length_ = s.Length();
    memcpy(sequenceData_->chars_, s.Data(), length_ * sizeof(CharType));
    return *this;
}

String &String::operator+=(const CharSequence &s)
{
    if (s.Empty())
    {
        return *this;
    }
    auto oldLen = length_;
    length_ = s.Length() + Length();
    if (Capacity() < length_)
    {
        ReAllocate(CalculateCapacity(length_));
    }
    memcpy(sequenceData_->chars_ + oldLen, s.Data(), s.Length() * sizeof(CharType));
    return *this;
}

String &String::operator=(const char *utf8)
{
    length_ = GetUtf8Length(utf8);
    if (Capacity() < length_)
    {
        ReAllocate(CalculateCapacity(length_));
    }
    uint32_t index = 0;
    auto readFn = [&index, utf8]() -> utf8::char_type { return (utf8::char_type)utf8[index++]; };

    for (uint32_t i = 0; i < length_; ++i)
    {
        sequenceData_->chars_[i] = utf8::read(readFn);
    }
    return *this;
}

String &String::operator+=(const char *utf8)
{
    auto utf8Len = GetUtf8Length(utf8);
    if (utf8Len == 0)
    {
        return *this;
    }
    auto oldLen = length_;
    length_ = utf8Len + oldLen;
    if (Capacity() < length_)
    {
        ReAllocate(CalculateCapacity(length_));
    }

    uint32_t index = 0;
    auto readFn = [&index, utf8]() -> utf8::char_type { return (utf8::char_type)utf8[index++]; };

    for (uint32_t i = 0; i < utf8Len; ++i)
    {
        sequenceData_->chars_[i + oldLen] = utf8::read(readFn);
    }
    return *this;
}

String &String::operator=(const wchar_t *unicode)
{
    length_ = (uint32_t)wcslen(unicode);
    if (Capacity() < length_)
    {
        ReAllocate(CalculateCapacity(length_));
    }
    for (uint32_t i = 0; i < length_; ++i)
    {
        sequenceData_->chars_[i] = unicode[i];
    }
    return *this;
}

String &String::operator+=(const wchar_t *unicode)
{
    auto len = (uint32_t)wcslen(unicode);
    if (len == 0)
    {
        return *this;
    }
    auto oldLen = length_;
    length_ = len + oldLen;
    if (Capacity() < length_)
    {
        ReAllocate(CalculateCapacity(length_));
    }
    for (uint32_t i = 0; i < len; ++i)
    {
        sequenceData_->chars_[i + oldLen] = unicode[i];
    }
    return *this;
}

void String::Reserve(uint32_t n)
{
    if (Capacity() < n)
    {
        ReAllocate(CalculateCapacity(n));
    }
}

void String::Resize(uint32_t n)
{
    if (Capacity() < n)
    {
        ReAllocate(CalculateCapacity(n));
    }
    length_ = n;
}

void String::TrimLeading()
{
    for (uint32_t i = 0; i < Length(); ++i)
    {
        if (!IsWhiteSpace(At(i)))
        {
            length_ -= i;
            memmove(sequenceData_->chars_, sequenceData_->chars_ + i, Length() * sizeof(CharType));
            return;
        }
    }
    length_ = 0;
}

void String::TrimTrailing()
{
    for (uint32_t i = Length(); i > 0; --i)
    {
        if (!IsWhiteSpace(At(i - 1)))
        {
            length_ = i;
            return;
        }
    }
    length_ = 0;
}

void String::ReAllocate(uint32_t newCapacity)
{
    capacity_ = newCapacity;
    if (sequenceData_ == nullptr)
    {
        sequenceData_ = new SequenceData(capacity_);
    }
    else
    {
        sequenceData_->ReAllocate(capacity_);
    }
}

void String::Free()
{
    delete sequenceData_;
    sequenceData_ = nullptr;
    length_ = 0;
    capacity_ = 0;
}

std::ostream &operator<<(std::ostream &os, const CharSequence &s)
{
    os << s.ToStdString(CharSequence::kUtf8);
    return os;
}

} // namespace ss
