//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

namespace ss
{

class CharSequence;
class String;

class StrSplitter
{
public:
    StrSplitter(const CharSequence &str, const CharSequence &splitter) : str_(str), splitter_(splitter)
    {
    }

    template <class Container> void Split(Container &c) const;

    template <class Container>
    operator Container() const; // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

private:
    const CharSequence &str_;
    const CharSequence &splitter_;
};

}