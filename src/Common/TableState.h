#ifndef TABLESTATEMANAGER_H
#define TABLESTATEMANAGER_H

#include "Global.h"
#include "AbstractStateManager.h"

#define COLUMNS_NAME "COLUMNS"

class CTableState : public ICState
{
    ADD_QSMART_PTR(QSettings, m_settings, getSettings)

public:
    CTableState();
    ~CTableState();

    virtual QByteArray getState(const QString &tableName) override final;
    virtual void saveState(const QString &tableName, const QByteArray &state) override final;

    void clearState(const QString &tableName);

    void saveColumnsNames(const QString &tableName, QStringList columns);
    QStringList getColumnsNames(const QString &tableName);
    void clearColumnsNames(const QString &tableName);

};


#endif // TABLESTATEMANAGER_H
