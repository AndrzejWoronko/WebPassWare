#ifndef SINGLETON_H
#define SINGLETON_H

#include <stdlib.h>

// Mayers singleton template class

namespace wpw {

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

} // namespace wpw

#endif // SINGLETON_H
