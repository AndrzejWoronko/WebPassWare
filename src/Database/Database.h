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

class ICDatabase
{

public:

virtual ~ICDatabase() = default;

virtual bool createTable(const QString &tableName, const QList <PkNames> &info) = 0;
virtual bool createTableIndexes(const QString &tableName, const QHash<QString, QString> &info, bool unique) = 0;
virtual bool dropTable(const QString &tableName) = 0;
virtual bool dropTableIndexes(const QString &tableName, const QHash<QString, QString> &info) = 0;
virtual bool alterTableAddColumn(const QString &tableName, const PkNames &info) = 0;
virtual QList<QSqlIndex> getTableIndexes(const QString &tableName) = 0;

virtual qint64 addRecord(const QString &tableName, const QHash<QString, QVariant> &info, qint64 newId, bool force_id) = 0;
virtual bool updateRecord(const QString &tableName, const QHash<QString, QVariant> &info, const QString &params) = 0;
virtual bool replaceRecord(const QString &tableName, const QHash<QString, QVariant> &info) = 0;

virtual QList<QSqlRecord> find(const QString &tableName, const QString &fieldName, const QString &params) = 0;
virtual QSqlRecord first(const QString &tableName, const QString &pkName) = 0;
virtual QSqlRecord last(const QString &tableName, const QString &pkName) = 0;

virtual bool remove(const QString &tableName, const QString &params) = 0;
virtual bool setDeleted(const QString &tableName, const QString &params) = 0;

virtual qint64 count(const QString &tableName, const QString &arg) = 0;
virtual qint64 countBy(const QString &tableName, const QString &params) = 0;

virtual bool beginTransaction(void) = 0;
virtual bool commitTransaction(void) = 0;
virtual bool rollbackTransaction(void) = 0;

};


class CDatabase : public ICDatabase
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

    static void showErrorInfo(CException &e);

    void setQueryLog(void);
    void reConnect();

    bool checkTableInDatabase(const QString &tableName);
    QSqlRecord getTableFields(const QString &tableName);
    bool resetAutoIncrement(const QString &tableName, qint64 value);
    static void fillTableTypes(QHash<QString, QString> &fieldSqlTypes);

/*Interface to DB*/

    virtual bool createTable(const QString &tableName, const QList <PkNames> &info) override final;
    virtual bool createTableIndexes(const QString &tableName, const QHash<QString, QString> &info, bool unique = true) override final;
    virtual bool dropTable(const QString &tableName) override final;
    virtual bool dropTableIndexes(const QString &tableName, const QHash<QString, QString> &info) override final;
    virtual bool alterTableAddColumn(const QString &tableName, const PkNames &info) override final;
    virtual QList<QSqlIndex> getTableIndexes(const QString &tableName) override final;

    virtual qint64 addRecord(const QString &tableName, const QHash<QString, QVariant> &info, qint64 newId, bool force_id = false) override final;
    virtual bool updateRecord(const QString &tableName, const QHash<QString, QVariant> &info, const QString &params) override final;
    virtual bool replaceRecord(const QString &tableName, const QHash<QString, QVariant> &info) override final;

    virtual QList<QSqlRecord> find(const QString &tableName, const QString &fieldName, const QString &params) override final;
    virtual QSqlRecord first(const QString &tableName, const QString &pkName) override final;
    virtual QSqlRecord last(const QString &tableName, const QString &pkName) override final;

    virtual bool remove(const QString &tableName, const QString &params) override final;
    virtual bool setDeleted(const QString &tableName, const QString &params) override final;

    virtual qint64 count(const QString &tableName, const QString &arg) override final;
    virtual qint64 countBy(const QString &tableName, const QString &params) override final;

    virtual bool beginTransaction(void) override final;
    virtual bool commitTransaction(void) override final;
    virtual bool rollbackTransaction(void) override final;


private:

    static QString getFieldTypeNameSqlite(int t);

    bool m_conected;

    QSqlDatabase m_db;

    CSqlQuery *m_query;
    QString m_lastQuery;
};


using SingletonDatabase = SingletonHolder<CDatabase>;

#define DB SingletonDatabase::getInstance()


#endif // DATABASE_H

