//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Str.h"

namespace ss
{

template <class Container> StrSplitter::operator Container() const
{
    Container container;
    Split(container);
    return container;
}

template <class Container> void StrSplitter::Split(Container &container) const
{
    uint32_t indexStart = 0;
    while (true)
    {
        uint32_t indexEnd = str_.Find(splitter_, indexStart);
        if (indexEnd == CharSequence::kNPos)
        {
            container.insert(container.end(), str_.SubStringView(indexStart));
            break;
        }
        container.insert(container.end(), str_.SubStringView(indexStart, indexEnd - indexStart));
        indexStart = indexEnd + splitter_.Length();
    }
}

}