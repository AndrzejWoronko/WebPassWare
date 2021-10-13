#ifndef DATABASE_H
#define DATABASE_H

#include "Global.h"
#include <QtSql>
#include <QSettings>
#include <QSqlDatabase>

#define DB_CONNECTION_NAME "WebPassWareDbConnection"
#define DB_NAME "WebPassWare.db"

#define DB CDatabase::getInstance()

class CDatabase : public Singleton<CDatabase>
{
    ADD_FIELD(QString, m_base_name, getBaseName, setBaseName)
    ADD_FIELD(bool, m_echo_error, getEchoError, setEchoError)
    ADD_FIELD(bool, m_log_error, getLogError, setLogError)
    ADD_FIELD(bool, m_echo_query, getEchoQuery, setEchoQuery)
    ADD_FIELD(bool, m_log_query, getLogQuery, setLogQuery)

    QSettings settings;

public:

    CDatabase();
    ~CDatabase();

    void initConnection();
    bool isConnected();
    QSqlDatabase &getDb();

    void reConnect();

    bool checkTableInDatabase(const QString &tableName);
    QSqlRecord getTableFields(const QString &tableName);

private:

    static QString getFieldTypeNameSqlite(int t);

    bool m_conected;

    QSqlDatabase m_db;
};

#endif // DATABASE_H

