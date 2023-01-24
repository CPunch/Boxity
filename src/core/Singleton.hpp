#pragma once

#include <assert.h>

// make sure to add a
// !!!
// template <> T *Singleton<T>::singleton = nullptr;
// !!!
template<typename T> class Singleton {
protected:
    static T* singleton;

public:
    Singleton() {
        assert(singleton == nullptr);
        singleton = static_cast<T*>(this);
    }
    ~Singleton() {
        assert(singleton);
        singleton = nullptr;
    }

    static T& getSingleton() {
        assert(singleton);
        return (*singleton);
    }

    static T* getSingletonPtr() {
        assert(singleton);
        return singleton;
    }

    static bool isAlive() {
        return singleton != nullptr;
    }
};