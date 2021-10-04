#ifndef DIALOGSTATE_H
#define DIALOGSTATE_H

#include "Global.h"

class CDialogState
{

    ADD_PTR(QSettings, m_settings, getSettings)

public:
    CDialogState();
    ~CDialogState();

    void saveState(const QString &dialogName, QByteArray state);
    QByteArray getState(const QString &dialogName);

};

#endif // DIALOGSTATE_H
