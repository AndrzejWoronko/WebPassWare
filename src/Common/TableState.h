#ifndef TABLESTATEMANAGER_H
#define TABLESTATEMANAGER_H

#include "Global.h"

#define COLUMNS_NAME "COLUMNS"

class CTableState
{
    ADD_QSMART_PTR(QSettings, m_settings, getSettings)

public:
    CTableState();
    ~CTableState();

    void saveState(const QString &tableName, QByteArray state);
    QByteArray getState(const QString &tableName);
    void clearState(const QString &tableName);

    void saveColumnsNames(const QString &tableName, QStringList columns);
    QStringList getColumnsNames(const QString &tableName);
    void clearColumnsNames(const QString &tableName);

};


#endif // TABLESTATEMANAGER_H
