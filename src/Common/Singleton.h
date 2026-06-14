#ifndef SINGLETON_H
#define SINGLETON_H

#include <stdlib.h>

// Mayers singleton template class

template<typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T uniqueInstance;
        return uniqueInstance;
    }
};

///------------------------
/// Generic Singleton Holder
///
template <typename T>
class SingletonHolder
{
private:
    SingletonHolder() = default;
    ~SingletonHolder() = default;

public:
    SingletonHolder(const SingletonHolder&) = delete;
    SingletonHolder& operator=(const SingletonHolder&) = delete;

    static T& getInstance()
    {
        static T unique_instance;

        return unique_instance;
    }
};

#endif // SINGLETON_H
