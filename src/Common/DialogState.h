#ifndef DIALOGSTATE_H
#define DIALOGSTATE_H

#include "Global.h"
#include "AbstractStateManager.h"

class CDialogState : public ICState
{
    ADD_QSMART_UNIQUE_PTR(QSettings, m_settings, getSettings)

public:
    CDialogState();
     ~CDialogState();

    virtual void saveState(const QString &dialogName, const QByteArray &state) override final;
    virtual QByteArray getState(const QString &dialogName) override final;
};

#endif // DIALOGSTATE_H
