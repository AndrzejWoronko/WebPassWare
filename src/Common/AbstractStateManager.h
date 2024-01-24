#ifndef ABSTRACTSTATEMANAGER_H
#define ABSTRACTSTATEMANAGER_H
#include <Global.h>


class ICState
{
public:
    virtual ~ICState() = default;

    virtual QByteArray getState(const QString &) = 0;
    virtual void saveState(const QString &, const QByteArray &state) = 0;
};

class ICStateManager
{
public:
    virtual ~ICStateManager() = default;

    virtual void getState(const QString &, QByteArray &state) = 0;
    virtual void saveState(const QString &, const QByteArray &state) = 0;
};


#endif // ABSTRACTSTATEMANAGER_H
