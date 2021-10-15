#ifndef CSQLQUERY_H
#define CSQLQUERY_H

#include "Global.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class CSqlQuery : public QSqlQuery
{
    ADD_FIELD(bool, m_echo_error, getEchoError, setEchoError)
    ADD_FIELD(bool, m_log_error, getLogError, setLogError)
    ADD_FIELD(bool, m_echo_query, getEchoQuery, setEchoQuery)
    ADD_FIELD(bool, m_log_query, getLogQuery, setLogQuery)

public:
    CSqlQuery(const QString &query, QSqlDatabase &db);
    CSqlQuery(QSqlDatabase &db);

    bool exec();
    bool exec(const QString &query);
    void setLogFromDB(bool echo_error, bool log_error, bool echo_query, bool log_query);

    void setSqlCursor();
    void restoreSqlCursor();
private:
    void logEchoQuery();
    bool m_returnValue;
    QDateTime m_time;
};

#endif // CSQLQUERY_H
