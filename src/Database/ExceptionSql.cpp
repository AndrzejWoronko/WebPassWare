#include "ExceptionSql.h"

CExceptionSql::CExceptionSql(const QString &metoda, CSqlQuery query) : CException(metoda, query.lastError().databaseText())
{
    LOG_SQL << " ERROR EXCEPTION: " << "Metoda: " + metoda + " blad: " + query.lastError().databaseText() + " komenda: " + query.lastQuery();
    DEBUG_WITH_LINE  << " ERROR EXCEPTION: " << "Metoda: " + metoda + " blad: " + query.lastError().databaseText() + " komenda: " + query.lastQuery() << Qt::endl;
}

CExceptionSql::CExceptionSql(const QString &metoda, CSqlQuery *query) : CException(metoda, query->lastError().databaseText())
{
    LOG_SQL << " ERROR EXCEPTION: " << "Metoda: " + metoda + " blad: " + query->lastError().databaseText() + " komenda: " + query->lastQuery();
    DEBUG_WITH_LINE  << " ERROR EXCEPTION: " << "Metoda: " + metoda + " blad: " + query->lastError().databaseText() + " komenda: " + query->lastQuery() << Qt::endl;
}

CExceptionSql::CExceptionSql(const QString &metoda, const QString &query, const QString &error) : CException(metoda, error)
{
    LOG_SQL << " ERROR EXCEPTION: " << "Metoda: " + metoda + " blad: " + error + " komenda: " + query;
    DEBUG_WITH_LINE << " ERROR EXCEPTION: " << "Metoda: " + metoda + " blad: " + error + " komenda: " + query << Qt::endl;
}

CExceptionSql::~CExceptionSql()
{
}
