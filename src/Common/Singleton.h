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

#endif // SINGLETON_H
