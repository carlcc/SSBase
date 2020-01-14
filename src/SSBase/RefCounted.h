//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "Assert.h"

namespace ss
{

template <class T> class SharedPtr;
template <class T> class WeakPtr;

class RefCounted
{
private:
    struct RefCounter
    {
        RefCounted *object;
        int sharedCount;
        int weakCount;

        void IncreaseShared()
        {
            SSASSERT(sharedCount >= 0);
            ++sharedCount;
        }

        void DecreaseShared()
        {
            SSASSERT(sharedCount > 0);
            if (--sharedCount == 0)
            {
                delete object;
            }
        }

        void IncreaseWeak()
        {
            SSASSERT(weakCount >= 0);
            ++weakCount;
        }

        void DecreaseWeak()
        {
            SSASSERT(weakCount > 0);
            if (--weakCount == 0)
            {
                delete this;
            }
        }

        bool Expired()
        {
            return sharedCount == 0;
        }
    };

public:
    RefCounted() : refCounter_(new RefCounter)
    {
        refCounter_->object = this;
        refCounter_->sharedCount = 0;
        refCounter_->weakCount = 0;
        // This object has one weakCount
        refCounter_->IncreaseWeak();
    }

    virtual ~RefCounted()
    {
        refCounter_->DecreaseWeak();
    }

private:
    RefCounter *refCounter_;

    template <class T> friend class SharedPtr;
    template <class T> friend class WeakPtr;
};

}