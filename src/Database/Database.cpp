#include "Database.h"
#include "ApplicationSettings.h"

CDatabase::CDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", QLatin1String(DB_CONNECTION_NAME));
    initConnection();
    m_echo_error = true;
    m_log_error = true;
    m_echo_query = true;
    m_log_query = true;
}

CDatabase::~CDatabase()
{
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
    QFileInfo f(SETT.getSettings()->fileName());
    m_base_name = QString("%1/%2").arg(f.absolutePath()).arg(DB_NAME);
    DEBUG_WITH_LINE << m_base_name;

    m_db.setDatabaseName(m_base_name);

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
