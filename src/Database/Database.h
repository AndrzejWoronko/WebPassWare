#ifndef DATABASE_H
#define DATABASE_H

#include "Global.h"
#include <QSqlDatabase>
#include <QSqlIndex>
#include <QSqlRecord>
#include "SqlQuery.h"
#include "Exception.h"

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
virtual bool updateRecord(const QString &tableName, const QHash<QString, QVariant> &info, const QString &params, const QVariantHash &bindings = QVariantHash()) = 0;

virtual QList<QSqlRecord> find(const QString &tableName, const QString &fieldName, const QString &params, const QVariantHash &bindings = QVariantHash()) = 0;
virtual QSqlRecord first(const QString &tableName, const QString &pkName) = 0;
virtual QSqlRecord last(const QString &tableName, const QString &pkName) = 0;

virtual bool remove(const QString &tableName, const QString &params, const QVariantHash &bindings = QVariantHash()) = 0;
virtual bool setDeleted(const QString &tableName, const QString &params, const QVariantHash &bindings = QVariantHash()) = 0;

virtual qint64 count(const QString &tableName, const QString &arg) = 0;
virtual qint64 countBy(const QString &tableName, const QString &params) = 0;


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

    // A relative name is stored in AppLocalDataLocation; an absolute path is used directly.
    CDatabase(const QString &database_name = QString(DB_NAME));
    ~CDatabase();

    // The master password (salted hash in the settings file, see
    // wpw::MasterPassword). A legacy empty password requires setup.
    bool hasDatabasePassword();
    // True if \a passwd is the master password; an old MD5 hash is replaced
    // by the new form after a successful check.
    bool checkDatabasePassword(const QString &passwd);
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
    virtual bool updateRecord(const QString &tableName, const QHash<QString, QVariant> &info, const QString &params, const QVariantHash &bindings = QVariantHash()) override final;

    virtual QList<QSqlRecord> find(const QString &tableName, const QString &fieldName, const QString &params, const QVariantHash &bindings = QVariantHash()) override final;
    virtual QSqlRecord first(const QString &tableName, const QString &pkName) override final;
    virtual QSqlRecord last(const QString &tableName, const QString &pkName) override final;

    virtual bool remove(const QString &tableName, const QString &params, const QVariantHash &bindings = QVariantHash()) override final;
    virtual bool setDeleted(const QString &tableName, const QString &params, const QVariantHash &bindings = QVariantHash()) override final;

    virtual qint64 count(const QString &tableName, const QString &arg) override final;
    virtual qint64 countBy(const QString &tableName, const QString &params) override final;



private:

    static QString getFieldTypeNameSqlite(int t);
    // Prepares m_lastQuery, binds the values (placeholder -> value) and runs it.
    bool execPrepared(const QVariantHash &bindings);

    bool m_conected;

    QSqlDatabase m_db;

    CSqlQuery *m_query;
    QString m_lastQuery;
};



#endif // DATABASE_H
