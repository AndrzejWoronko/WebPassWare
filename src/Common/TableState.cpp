#include "TableState.h"
#include "Application.h"

CTableState::CTableState()
{
    APPI->setAppInformation();
    m_settings = std::make_shared<QSettings>();
    m_settings->beginGroup("TableStates");
}

CTableState::~CTableState()
{
    m_settings->endGroup();
    //safe_delete(m_settings)
}

void CTableState::saveState(const QString &tableName, QByteArray state)
{
    m_settings->setValue(tableName, state);
}

QByteArray CTableState::getState(const QString &tableName)
{
    return m_settings->value(tableName).toByteArray();
}

void CTableState::clearState(const QString &tableName)
{
    m_settings->remove(tableName);
}

void CTableState::saveColumnsNames(const QString &tableName, QStringList columns)
{
    QString name = QString("%1_%2").arg(tableName).arg(COLUMNS_NAME);
    m_settings->setValue(name, columns);
}

QStringList CTableState::getColumnsNames(const QString &tableName)
{
    QString name = QString("%1_%2").arg(tableName).arg(COLUMNS_NAME);
    return  m_settings->value(name).toStringList();
}

void CTableState::clearColumnsNames(const QString &tableName)
{
    QString name = QString("%1_%2").arg(tableName).arg(COLUMNS_NAME);
    m_settings->remove(name);
}
