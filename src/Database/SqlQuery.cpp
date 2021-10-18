#include <QApplication>
#include "SqlQuery.h"
#include "Logger.h"

CSqlQuery::CSqlQuery(QSqlDatabase db) :  QSqlQuery(QString(), db),
    m_echo_error(true), m_log_error(true), m_echo_query(true), m_log_query(true)
{
    m_returnValue = false;
}

CSqlQuery::CSqlQuery(const QString &pSql, QSqlDatabase db) : QSqlQuery(QString(), db),
    m_echo_error(true), m_log_error(true), m_echo_query(true), m_log_query(true)
{
    QString query = pSql;
    setSqlCursor();
    m_returnValue = QSqlQuery::exec(query);
    logEchoQuery();
    restoreSqlCursor();
}

bool CSqlQuery::exec()
{
    QMutexLocker lock(&m_queryMutex);

    setSqlCursor();
    m_returnValue = QSqlQuery::exec();
    logEchoQuery();
    restoreSqlCursor();
    return m_returnValue;
}

bool CSqlQuery::exec(const QString &pSql)
{
    QMutexLocker lock(&m_queryMutex);

    QString query = pSql;
    setSqlCursor();
    m_returnValue = QSqlQuery::exec(query);
    logEchoQuery();
    restoreSqlCursor();
    return m_returnValue;
}

void CSqlQuery::logEchoQuery()
{
    if (m_echo_query)
    {
        DEBUG_WITH_LINE << QSqlQuery::lastQuery();
    }
    if (m_log_query)
    {
        LOG_SQL << QSqlQuery::lastQuery();
    }

    if (QSqlQuery::lastError().isValid())
    {
        if (m_echo_error)
            DEBUG_WITH_LINE << "ERROR QUERY: " << QSqlQuery::lastQuery() << QSqlQuery::lastError().databaseText();
        if (m_log_error)
            LOG_SQL << "ERROR QUERY: " << QSqlQuery::lastQuery() << QSqlQuery::lastError().databaseText();
    }
}
void CSqlQuery::setLogFromDB(bool echo_error, bool log_error, bool echo_query, bool log_query)
{
    setEchoError(echo_error);
    setLogError(log_error);
    setEchoQuery(echo_query);
    setLogQuery(log_query);
}

void CSqlQuery::setSqlCursor()
{
    m_time = QDateTime::currentDateTime();
    if (qApp)
       qApp->setOverrideCursor(QCursor(Qt::BusyCursor));
}

void CSqlQuery::restoreSqlCursor()
{
    if (qApp)
        qApp->restoreOverrideCursor();
    DEBUG_WITH_LINE << "Query time: (" << m_time.msecsTo(QDateTime::currentDateTime())/1000.0 << "sec)";
}
