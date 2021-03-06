#ifndef DATABASE_H
#define DATABASE_H

#include "Global.h"
#include <QtSql>
#include <QSqlDatabase>
#include "SqlQuery.h"
#include "ExceptionSql.h"

#define DB_CONNECTION_NAME "WebPassWareDbConnection"
#define DB_NAME "WebPassWare.db"

#define DATABASE_SETTING QString::fromUtf8("Database/")
#define DATABASE_SETTING_PASSWORD DATABASE_SETTING + "Password"

#define DB CDatabase::getInstance()

class CDatabase : public Singleton<CDatabase>
{
    ADD_FIELD(QString, m_base_name, getBaseName, setBaseName)
    ADD_FIELD(QString, m_base_name_full_path, getBaseNameFullPath, setBaseNameFullPath)
    ADD_FIELD(bool, m_echo_error, getEchoError, setEchoError)
    ADD_FIELD(bool, m_log_error, getLogError, setLogError)
    ADD_FIELD(bool, m_echo_query, getEchoQuery, setEchoQuery)
    ADD_FIELD(bool, m_log_query, getLogQuery, setLogQuery)

public:

    CDatabase(const QString &database_name = QString(DB_NAME));
    ~CDatabase();

    QString getDatabaseHashPassword();
    void setDatabaseHashPassword(const QString &hash_passwd);
    void setDatabasePassword(const QString &passwd);

    void initConnection();
    bool isConnected();
    QSqlDatabase &getDb();
    CSqlQuery *getQuery();

    static void showErrorInfo(CException *e);

    void setQueryLog(void);
    void reConnect();

    bool checkTableInDatabase(const QString &tableName);
    QSqlRecord getTableFields(const QString &tableName);

    static void fillTableTypes(QHash<QString, QString> &fieldSqlTypes);
    bool createTable(const QString &tableName, const QList <PkNames> &info);
    bool dropTable(const QString &tableName);
    QList<QSqlIndex> getTableIndexes(const QString &tableName);
    bool createTableIndexes(const QString &tableName, const QHash<QString, QString> &info, bool unique);
    bool dropTableIndexes(const QString &tableName, const QHash<QString, QString> &info);
    bool alterTableAddColumn(const QString &tableName, const PkNames &info);
    bool resetAutoIncrement(const QString &tableName, qint64 value);

    qint64 addRecord(const QString &tableName, const QHash<QString, QVariant> &info, qint64 newId, bool force_id = false);
    bool updateRecord(const QString &tableName, const QHash<QString, QVariant> &info, const QString &params);
    bool replaceRecord(const QString &tableName, const QHash<QString, QVariant> &info);
    bool remove(const QString &tableName, const QString &params);
    bool setDeleted(const QString &tableName, const QString &params);

    QList<QSqlRecord> find(const QString &tableName, const QString &fieldName, const QString &params);
    QSqlRecord first(const QString &tableName, const QString &pkName);
    QSqlRecord last(const QString &tableName, const QString &pkName);

    qint64 count(const QString &tableName, const QString &arg);
    qint64 countBy(const QString &tableName, const QString &params);

    bool beginTransaction(void);
    bool commitTransaction(void);
    bool rollbackTransaction(void);
private:

    static QString getFieldTypeNameSqlite(int t);

    bool m_conected;

    QSqlDatabase m_db;

    CSqlQuery *m_query;
    QString m_lastQuery;
};

#endif // DATABASE_H

