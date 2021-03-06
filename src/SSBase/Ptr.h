//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "RefCounted.h"
#include <functional>
#include <memory>

namespace ss {

template <class T>
class SharedPtr {
public:
    SharedPtr()
        : refCounter_(nullptr)
    {
    }

    SharedPtr(decltype(nullptr)) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : refCounter_(nullptr)
    {
    }

    SharedPtr(T* t)
        : refCounter_(nullptr) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        if (t != nullptr) {
            refCounter_ = static_cast<RefCounted*>(t)->refCounter_;
            SSASSERT(refCounter_ != nullptr);
            refCounter_->IncreaseShared();
        }
    }

    template <class P>
    SharedPtr(const SharedPtr<P>& p) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : refCounter_(p.refCounter_)
    {
        (void)static_cast<T*>(p.Get()); // to ensure type T and P are compatible
        IncreaseRefCount();
    }

    SharedPtr(const SharedPtr& p)
        : refCounter_(p.refCounter_)
    {
        IncreaseRefCount();
    }

    SharedPtr(SharedPtr&& p) noexcept
        : refCounter_(p.refCounter_)
    {
        p.refCounter_ = nullptr;
    }

    ~SharedPtr()
    {
        DecreaseRefCount();
    }

    T* Get() const
    {
        if (refCounter_ == nullptr) {
            return nullptr;
        }
        return static_cast<T*>(refCounter_->object);
    }

    explicit operator bool() const
    {
        return Get() != nullptr;
    }

    bool operator!() const
    {
        return Get() == nullptr;
    }

    operator T*() const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return Get();
    }

    T* operator->() const
    {
        return Get();
    }

    T& operator*() const
    {
        return *Get();
    }

    SharedPtr& operator=(const SharedPtr& p) noexcept
    {
        if (p.refCounter_ != refCounter_) {
            DecreaseRefCount();
            refCounter_ = p.refCounter_;
            IncreaseRefCount();
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& p) noexcept
    {
        if (p.refCounter_ != refCounter_) {
            DecreaseRefCount();
            refCounter_ = p.refCounter_;
            p.refCounter_ = nullptr;
        }
        return *this;
    }

    template <class P>
    SharedPtr& operator=(const SharedPtr<P>& p)
    {
        (void)static_cast<T*>(p.Get()); // static_cast to ensure type T and P are compatible
        if (refCounter_ != p.refCounter_) {
            DecreaseRefCount();
            refCounter_ = p.refCounter_;
            IncreaseRefCount();
        }
        return *this;
    }

private:
    SharedPtr(RefCounted::RefCounter* refCounter) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : refCounter_(refCounter)
    {
        IncreaseRefCount();
    }

    void IncreaseRefCount()
    {
        if (refCounter_ != nullptr) {
            refCounter_->IncreaseShared();
        }
    }
    void DecreaseRefCount()
    {
        if (refCounter_ != nullptr) {
            refCounter_->DecreaseShared();
        }
    }

private:
    RefCounted::RefCounter* refCounter_;

    template <class P>
    friend class SharedPtr;
    template <class P>
    friend class WeakPtr;
    template <class Q, class P>
    friend bool operator==(const SharedPtr<Q>& t, const SharedPtr<P>& p);
    template <class Q, class P>
    friend bool operator!=(const SharedPtr<Q>& t, const SharedPtr<P>& p);
};

template <class T>
class WeakPtr {
public:
    WeakPtr()
        : refCounter_(nullptr)
    {
    }

    WeakPtr(T* p)
        : refCounter_(nullptr) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        if (p != nullptr) {
            refCounter_ = static_cast<RefCounted*>(p)->refCounter_;
            SSASSERT(refCounter_ != nullptr);
            refCounter_->IncreaseWeak();
        }
    }

    WeakPtr(const SharedPtr<T>& p) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : refCounter_(p.refCounter_)
    {
        IncreaseRefCount();
    }

    template <class P>
    WeakPtr(const SharedPtr<P>& p) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : refCounter_(p.refCounter_)
    {
        (void)static_cast<T*>(p.Get()); // static_cast to ensure type T and P are compatible
        IncreaseRefCount();
    }

    template <class P>
    WeakPtr(const WeakPtr<P>& p) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : refCounter_(p.refCounter_)
    {
        (void)static_cast<T*>(p.Get()); // to ensure type T and P are compatible
        IncreaseRefCount();
    }

    WeakPtr(const WeakPtr& p)
        : refCounter_(p.refCounter_)
    {
        IncreaseRefCount();
    }

    WeakPtr(WeakPtr&& p) noexcept
        : refCounter_(p.refCounter_)
    {
        p.refCounter_ = nullptr;
    }

    ~WeakPtr()
    {
        DecreaseRefCount();
    }

    SharedPtr<T> Lock() const
    {
        if (refCounter_ == nullptr || refCounter_->Expired()) {
            return nullptr;
        }
        return SharedPtr<T>(refCounter_);
    }

    WeakPtr& operator=(const WeakPtr& p)
    {
        if (p.refCounter_ != refCounter_) {
            DecreaseRefCount();
            refCounter_ = p.refCounter_;
            IncreaseRefCount();
        }
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& p) noexcept
    {
        if (p.refCounter_ != refCounter_) {
            DecreaseRefCount();
            refCounter_ = p.refCounter_;
            p.refCounter_ = nullptr;
        }
        return *this;
    }

    template <class P>
    WeakPtr& operator=(const WeakPtr<P>& p)
    {
        (void)static_cast<T*>(p.Get()); // static_cast to ensure type T and P are compatible
        if (p.refCounter_ != refCounter_) {
            DecreaseRefCount();
            refCounter_ = p.refCounter_;
            IncreaseRefCount();
        }
        return *this;
    }

private:
    void IncreaseRefCount()
    {
        if (refCounter_ != nullptr) {
            refCounter_->IncreaseWeak();
        }
    }
    void DecreaseRefCount()
    {
        if (refCounter_ != nullptr) {
            refCounter_->DecreaseWeak();
        }
    }

private:
    RefCounted::RefCounter* refCounter_;
    template <class P>
    friend class WeakPtr;
    template <class Q, class P>
    friend bool operator!=(const WeakPtr<Q>& t, const WeakPtr<P>& p);
    template <class Q, class P>
    friend bool operator!=(const WeakPtr<Q>& t, const WeakPtr<P>& p);
};

template <class T, class P>
inline bool operator==(const SharedPtr<T>& t, const SharedPtr<P>& p)
{
    return t.refCounter_ == p.refCounter_;
}

template <class T, class P>
inline bool operator!=(const SharedPtr<T>& t, const SharedPtr<P>& p)
{
    return t.refCounter_ != p.refCounter_;
}

template <class T, class P>
inline bool operator==(const SharedPtr<T>& t, const P* p)
{
    return t.Get() == p;
}

template <class T>
inline bool operator==(const SharedPtr<T>& t, decltype(nullptr))
{
    return t.Get() == nullptr;
}

template <class T>
inline bool operator==(decltype(nullptr), const SharedPtr<T>& t)
{
    return t.Get() == nullptr;
}

template <class T, class P>
inline bool operator!=(const SharedPtr<T>& t, const P* p)
{
    return t.Get() != p;
}

template <class T, class P>
inline bool operator==(const P* p, const SharedPtr<T>& t)
{
    return t.Get() == p;
}

template <class T, class P>
inline bool operator!=(const P* p, const SharedPtr<T>& t)
{
    return t.Get() != p;
}

template <class T, class P>
inline bool operator==(const WeakPtr<T>& t, const WeakPtr<P>& p)
{
    return t.refCounter_ == p.refCounter_;
}

template <class T, class P>
inline bool operator!=(const WeakPtr<T>& t, const WeakPtr<P>& p)
{
    return t.refCounter_ != p.refCounter_;
}

template <class T, class... Args>
inline SharedPtr<T> MakeShared(Args&&... args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
void _ScopedPtrDefaultDestructor(T* t)
{
    delete t;
}

} // namespace ss