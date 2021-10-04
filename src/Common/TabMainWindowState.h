#ifndef CTABMAINWINDOWSTATE_H
#define CTABMAINWINDOWSTATE_H

#include "Global.h"

class CTabMainWindowState
{

    ADD_PTR(QSettings, m_settings, getSettings)

public:
    CTabMainWindowState();
    ~CTabMainWindowState();

    void saveState(const QString &tabMainWindowName, QByteArray state);
    QByteArray getState(const QString &tabMainWindowName);

};


#endif // CTABMAINWINDOWSTATE_H
