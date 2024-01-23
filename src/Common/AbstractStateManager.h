#ifndef ABSTRACTSTATEMANAGER_H
#define ABSTRACTSTATEMANAGER_H
#include <Global.h>

class ICStateManager
{
public:
    virtual ~ICStateManager() = default;

    virtual void getState(const QString &, QByteArray &state) = 0;
    virtual void saveState(const QString &, const QByteArray &state) = 0;
};


#endif // ABSTRACTSTATEMANAGER_H
