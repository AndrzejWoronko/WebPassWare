#include "Database.h"
#include "ApplicationSettings.h"

CDatabase::CDatabase(const QString &database_name): m_base_name(database_name)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", QLatin1String(DB_CONNECTION_NAME));
    initConnection();
    m_echo_error = true;
    m_log_error = true;
    m_echo_query = true;
    m_log_query = true;
    m_query = new CSqlQuery(m_db);
    m_query->setLogFromDB(m_echo_error, m_log_error, m_echo_query, m_log_query);
}

CDatabase::~CDatabase()
{
    safe_delete(m_query)
    if (m_db.isOpen())
       {
          DEBUG_WITH_LINE << "Database close connection" << Qt::endl;
          m_db.close();
       }
    DEBUG_WITH_LINE << "Database remove: " << DB_CONNECTION_NAME;
    m_db.removeDatabase(QLatin1String(DB_CONNECTION_NAME));
}

void CDatabase::initConnection()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QFileInfo f(dir);
    if (!f.exists())
       {
            QDir d;
            d.mkpath(dir);
       }
//    QFileInfo f(SETT.getSettings()->fileName());
//    QString dir = f.absolutePath()
    m_base_name_full_path =  QString("%1/%2").arg(dir, m_base_name);
    DEBUG_WITH_LINE << "DB file path is: " << m_base_name_full_path;
    m_db.setDatabaseName(m_base_name_full_path);

    m_conected = m_db.open();
    if (m_conected)
        {
            DEBUG_WITH_LINE << "Connected";
            //Print all tables
            DEBUG_WITH_LINE << m_db.tables();
            if (m_db.tables().count() == 0)
                DEBUG_WITH_LINE << "Error: " << m_db.lastError().text();
        }
    else
        {
        DEBUG_WITH_LINE << "not connected";
        DEBUG_WITH_LINE << "Errror: " << m_db.lastError().text();
        }
}

bool CDatabase::isConnected()
{
    return m_conected;
}

void CDatabase::reConnect()
{
    if (m_db.isOpen())
       {
        m_db.close();
       }
    m_conected = m_db.open();
    if (!m_conected)
       {
         DEBUG_WITH_LINE << "not connected";
         DEBUG_WITH_LINE << "Error: " << m_db.lastError().text();
         //throw new CExceptionSql(QString(Q_FUNC_INFO), QObject::tr("not connected"), m_db.lastError().text());
       }
    else
       {
         DEBUG_WITH_LINE << "Connected";
         //Print all tables
         DEBUG_WITH_LINE << m_db.tables();
         if (m_db.tables().count() == 0)
            DEBUG_WITH_LINE << "Error: " << m_db.lastError().text();
       }
}

QSqlDatabase &CDatabase::getDb()
{
  return m_db;
}

CSqlQuery *CDatabase::getQuery()
{
    return m_query;
}

void CDatabase::showErrorInfo(CException *e)
{
    DEBUG_WITH_LINE << "Błąd zapisu w metodzie: "  << e->getSourceMethod();
    DEBUG_WITH_LINE << "Message: " << e->getMessage();
}

void CDatabase::setQueryLog(void)
{
    if (m_query)
    {
        m_query->setEchoError(m_echo_error);
        m_query->setLogError(m_log_error);
        m_query->setEchoQuery(m_echo_query);
        m_query->setLogQuery(m_log_query);
    }
}

QString CDatabase::getFieldTypeNameSqlite(int t)
{
    switch (t)
    {
        case 1:	return QString("integer");
        case 2:	return QString("float");
        case 3:	return QString("text");
        case 4:	return QString("blob");
        case 5:	return QString("null");
        default: return QString("Type %1").arg(t);
    }
}

bool CDatabase::checkTableInDatabase(const QString &tableName)
{
    QSqlRecord desc = m_db.record(tableName);
    return !desc.isEmpty();
}

QSqlRecord CDatabase::getTableFields(const QString &tableName)
{
    QSqlRecord desc = m_db.record(tableName);
    return desc;
}

void CDatabase::fillTableTypes(QHash<QString, QString> &fieldSqlTypes)
{
    fieldSqlTypes.insert("bool", "BOOL NOT NULL DEFAULT FALSE ");
    fieldSqlTypes.insert("int", "INT NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("uint", "INT UNSIGNED NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("qlonglong", "BIGINT NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("qint64", "BIGINT NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("qulonglong", "BIGINT UNSIGNED NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("double", "DOUBLE NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("QByteArray", "LONGBLOB");
    fieldSqlTypes.insert("QChar", "CHAR(1) NOT NULL DEFAULT '' ");
    fieldSqlTypes.insert("QDate", "DATE NOT NULL DEFAULT '0000-00-00' ");
    fieldSqlTypes.insert("QTime", "TIME NOT NULL DEFAULT '00:00:00' ");
    fieldSqlTypes.insert("QDateTime", "DATETIME NOT NULL DEFAULT '0000-00-00 00:00:00' ");
    fieldSqlTypes.insert("QString", "LONGTEXT NOT NULL DEFAULT '' ");
    fieldSqlTypes.insert("Value", "DECIMAL (10,2) NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("Price", "DECIMAL (10,2) NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("Amount", "DECIMAL (10,3) NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("Indicator", "DECIMAL (10,4) NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("Percent", "DECIMAL (10,2) NOT NULL DEFAULT 0 ");
    fieldSqlTypes.insert("Varchar255", "VARCHAR(255) NOT NULL DEFAULT '' ");
    fieldSqlTypes.insert("Timestamp", "TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ");
    fieldSqlTypes.insert("signed char", "TINYINT NOT NULL DEFAULT 0");
    fieldSqlTypes.insert("short", "SMALLINT NOT NULL DEFAULT 0");
}

bool CDatabase::createTable(const QString &tableName, const QList <PkNames> &info)
{
    m_lastQuery = QString("CREATE TABLE %1( id_%2 INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL, ").arg(tableName, tableName);

    Q_FOREACH(auto field, info)
        m_lastQuery += QString("%1 %2, ").arg(field.first, field.second);

    m_lastQuery.chop(2);
    m_lastQuery += QString(");");
    m_query->clear();
    if (m_query->exec(m_lastQuery))
       return true;
    else
       throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}

bool CDatabase::dropTable(const QString &tableName)
{
    m_lastQuery = QString("DROP TABLE IF EXISTS %1;").arg(tableName);

    m_query->clear();
    if (m_query->exec(m_lastQuery))
        return true;
    else
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}

QList<QSqlIndex> CDatabase::getTableIndexes(const QString &tableName)
{
    QList<QSqlIndex> result;
    QList<QSqlRecord> recs;

    m_lastQuery = QString("PRAGMA index_list(%1);").arg(tableName);
    m_query->clear();

    if(m_query->exec(m_lastQuery))
    {
        while(m_query->next())
            recs.append(m_query->record());
    }
    else
    {
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    }

    Q_FOREACH(auto i, recs)
    {
        m_lastQuery = QString("PRAGMA index_info(%1);").arg(i.value("name").toString());
        if(m_query->exec(m_lastQuery))
        {
            while(m_query->next())
            {
                QSqlIndex idx;
                idx.setCursorName(i.value("name").toString());
                idx.setName(m_query->record().value("name").toString());
                result.append(idx);
            }
        }
        else
        {
            throw new CExceptionSql(Q_FUNC_INFO, m_query);
        }
    }
    return result;
}

// In info fields of indexes
bool CDatabase::createTableIndexes(const QString &tableName, const QHash<QString, QString> &info, bool unique)
{
    bool ret = true;

    Q_FOREACH(auto name, info.keys())
    {
        m_lastQuery = QString("");
        if (unique)
            m_lastQuery += QString(" CREATE UNIQUE INDEX %1_%2_idx ON %3 (%4); ")
                               .arg(tableName, name, tableName, info.value(name));
        else
            m_lastQuery += QString(" CREATE INDEX %1_%2_idx ON %3 (%4); ")
                               .arg(tableName, name, tableName, info.value(name));
        ret = m_query->exec(m_lastQuery);
        if (!ret)
        {
            throw new CExceptionSql(Q_FUNC_INFO, m_query);
        }
    }
    return ret;
}


bool CDatabase::dropTableIndexes(const QString &tableName, const QHash<QString, QString> &info)
{
    bool ret = true;

    Q_FOREACH(auto name, info.keys())
    {
        m_lastQuery = QString("");
        m_lastQuery += QString(" DROP INDEX %1_%2_idx ON %3; ").arg(tableName, name, tableName);
        ret = m_query->exec(m_lastQuery); // Przy usunięciu indeksów trzeba do końca zrobić pętle i nie może być wyrzucenie wyjątku
        if (!ret)
        {
            throw new CExceptionSql(Q_FUNC_INFO, m_query);
            DEBUG_WITH_LINE  << " DROP INDEX ERROR : " << " blad: " + m_query->lastError().databaseText() + " komenda: " + m_query->lastQuery() << Qt::endl;
        }
    }
    return ret;
}

bool CDatabase::alterTableAddColumn(const QString &tableName, const PkNames &info)
{
    m_lastQuery = QString("ALTER TABLE %1 ADD COLUMN %2 %3;").arg(tableName, info.first, info.second);
    if (m_query->exec(m_lastQuery))
        return true;
    else
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}

qint64 CDatabase::addRecord(const QString &tableName, const QHash<QString, QVariant> &info, qint64 newId)
{
    QString key, fields, values;
    qint64 id = -1;

    if(info.isEmpty())
        m_lastQuery = QString("INSERT INTO %1 DEFAULT VALUES;").arg(tableName);
    else
    {
        if (newId > 0)
            {
              id = newId;
              fields = QString("id_%1").arg(tableName) + ", ";
              values = ":" + QString("id_%1").arg(tableName) + ", ";
            }
        else
            {
                id = 0;
            }

        m_lastQuery = QString("INSERT INTO %1(%2) VALUES(%3);");

        Q_FOREACH(auto key, info.keys())
        {
            fields += key + ", ";
            values += ":" + key + ", ";
        }
        if(!info.isEmpty())
        {
            fields.chop(2);
            values.chop(2);
        }
        m_lastQuery = m_lastQuery.arg(tableName, fields, values);
        m_query->prepare(m_lastQuery);

        if (newId > 0)
        {
            key = QString("id_%1").arg(tableName);
            m_query->bindValue(":" + key, id);
        }
        //Rest of values
        Q_FOREACH(auto key, info.keys())
            m_query->bindValue(":" + key, info.value(key, QVariant()));

        if(m_query->exec())
        {
            id = m_query->lastInsertId().toLongLong();
            return id;
        }
        else
        {
            throw new CExceptionSql(Q_FUNC_INFO, m_query);
            id = -1;
        }
    }
    return id;
}

bool CDatabase::updateRecord(const QString &tableName, const QHash<QString, QVariant> &info, const QString &params)
{
    m_lastQuery = QString("UPDATE %1 SET ").arg(tableName);

    Q_FOREACH(auto fieldName, info.keys())
        m_lastQuery += QString("%1 = :%1, ").arg(fieldName);
    m_lastQuery.chop(2);
    m_lastQuery += QString(" %1").arg(params);
    m_query->prepare(m_lastQuery);

    Q_FOREACH(auto fieldName, info.keys())
        m_query->bindValue(":" + fieldName, info.value(fieldName, QVariant()));

    if (m_query->exec())
        return true;
    else
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}

bool CDatabase::replaceRecord(const QString &tableName, const QHash<QString, QVariant> &info)
{
    m_lastQuery = QString("REPLACE %1 SET ").arg(tableName);

    Q_FOREACH(auto fieldName , info.keys())
        m_lastQuery += QString("%1 = :%1, ").arg(fieldName);
    m_lastQuery.chop(2);
    m_query->prepare(m_lastQuery);

    Q_FOREACH(auto fieldName, info.keys())
        m_query->bindValue(":" + fieldName, info.value(fieldName, QVariant()));

    if (m_query->exec())
        return true;
    else
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}

bool CDatabase::remove(const QString &tableName, const QString &params)
{
    m_lastQuery = QString("DELETE FROM %1 %2;").arg(tableName, params);
    if (m_query->exec(m_lastQuery))
        return true;
    else
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}

bool CDatabase::setDeleted(const QString &tableName, const QString &params)
{
    m_lastQuery = QString("UPDATE %1 SET deleted = 'true' %2;").arg(tableName, params);
    if (m_query->exec(m_lastQuery))
        return true;
    else
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}


QList<QSqlRecord> CDatabase::find(const QString &tableName, const QString &fieldName, const QString &params)
{
    QList<QSqlRecord> result;

    result.clear();
    m_lastQuery = QString("SELECT %1 FROM %2 %3;").arg(fieldName, tableName, params);
    if(m_query->exec(m_lastQuery))
    {        
        while(m_query->next())
            result.append(m_query->record());
    }
    else
    {
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    }
    return result;
}

QSqlRecord CDatabase::first(const QString &tableName, const QString &pkName)
{
    m_lastQuery = QString("SELECT * FROM %1 ORDER BY %2 ASC LIMIT 1;").arg(tableName, pkName);
    if (m_query->exec(m_lastQuery))
    {
        m_query->next();
        return m_query->record();
    }
    else
    {
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    }
    return QSqlRecord();
}

QSqlRecord CDatabase::last(const QString &tableName, const QString &pkName)
{
    m_lastQuery = QString("SELECT * FROM %1 ORDER BY %2 DESC LIMIT 1;").arg(tableName, pkName);
    if (m_query->exec(m_lastQuery))
    {
        m_query->next();
        return m_query->record();
    }
    else
    {
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    }
    return QSqlRecord();
}

qint64 CDatabase::count(const QString &tableName, const QString &params)
{
    m_lastQuery = QString("SELECT COUNT(%1) FROM %2;").arg(params, tableName);
    if(m_query->exec(m_lastQuery))
    {
        m_query->next();
        return m_query->value(0).toLongLong();
    }
    else
    {
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    }
    return -1;
}

qint64 CDatabase::countBy(const QString &tableName, const QString &params)
{
    m_lastQuery = QString("SELECT COUNT(*) FROM %1 %2;").arg(tableName, params);
    if(m_query->exec(m_lastQuery))
    {
        m_query->next();
        return m_query->value(0).toLongLong();
    }
    else
    {
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    }
    return -1;
}

bool CDatabase::beginTransaction(void)
{
    if (m_query->exec(QString("BEGIN EXCLUSIVE TRANSACTION;")))
        return true;
    else
    {
        //Jeżeli już jest rozpoczęta transkacja to nie wyrzucamy wyjątku bo commit będzie w innym miejscu
        if (m_query->lastError().nativeErrorCode().toInt() == 1)
            return true;
        else
            throw new CExceptionSql(Q_FUNC_INFO, m_query);
    }
    return false;
}

bool CDatabase::commitTransaction(void)
{
    if (m_query->exec(QString("COMMIT;")))
        return true;
    else
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}

bool CDatabase::rollbackTransaction(void)
{
    if (m_query->exec(QString("ROLLBACK;")))
        return true;
    else
        throw new CExceptionSql(Q_FUNC_INFO, m_query);
    return false;
}

