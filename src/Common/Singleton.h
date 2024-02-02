#ifndef SINGLETON
#define SINGLETON

#include <stdlib.h>

template <typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        if (0 == m_instance)
        {
            m_instance = new T();
            ::atexit( destroy );
        }
        else
        {

        };
        return *m_instance;
    }
private:

    static T* m_instance;

private:
    static void destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }
};

template <typename T>
T* Singleton< T >::m_instance = nullptr;


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

#endif // SINGLETON

