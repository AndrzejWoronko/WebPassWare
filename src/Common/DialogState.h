#ifndef DIALOGSTATE_H
#define DIALOGSTATE_H

#include "Global.h"

class CDialogState
{

    ADD_UNIQUE_SMART_PTR(QSettings, m_settings, getSettings)
    //std::unique_ptr<QSettings> m_settings;

public:
    CDialogState();
     ~CDialogState();

    void saveState(const QString &dialogName, QByteArray state);
    QByteArray getState(const QString &dialogName);
    //std::unique_ptr<QSettings>& getSettings();

};

#endif // DIALOGSTATE_H
