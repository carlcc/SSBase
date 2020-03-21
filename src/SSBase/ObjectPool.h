//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once
#include <mutex>
#include <queue>

namespace ss {

class SingleThreadPolicy {
public:
    static void Lock()
    {
    }

    static void Unlock()
    {
    }
};

class MultiThreadPolicy {
public:
    static std::mutex& GetMutex()
    {
        static std::mutex mutex;
        return mutex;
    }

    static void Lock()
    {
        GetMutex().lock();
    }

    static void Unlock()
    {
        GetMutex().unlock();
    }
};

template <class T, class ThreadPolicy = SingleThreadPolicy>
class ObjectPool {
public:
    explicit ObjectPool(size_t capacity)
        : cap_(capacity)
        , queue_()
    {
    }

    T* Get()
    {
        ThreadPolicy::Lock();
        if (queue_.size() == 0) {
            ThreadPolicy::Unlock();
            return new T;
        } else {
            T* t = queue_.front();
            queue_.pop();
            ThreadPolicy::Unlock();
            return t;
        }
    }

    void Put(T* t)
    {
        ThreadPolicy::Lock();
        if (queue_.size() >= cap_) {
            ThreadPolicy::Unlock();
            delete t;
        } else {
            queue_.push(t);
            ThreadPolicy::Unlock();
        }
    }

    size_t cap_;
    std::queue<T> queue_;
};

} // namespace ss