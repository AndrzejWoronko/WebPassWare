#include "ExceptionSql.h"

CExceptionSql::CExceptionSql(const QString &metoda, CSqlQuery query) : CException(metoda, query.lastError().databaseText())
{
    LOG_SQL << " ERROR EXCEPTION: " << "Method: " + metoda + " Error: " + query.lastError().databaseText() + " Query: " + query.lastQuery();
    DEBUG_WITH_LINE  << " ERROR EXCEPTION: " << "Method: " + metoda + " Error: " + query.lastError().databaseText() + " Query: " + query.lastQuery() << Qt::endl;
}

CExceptionSql::CExceptionSql(const QString &metoda, CSqlQuery *query) : CException(metoda, query->lastError().databaseText())
{
    LOG_SQL << " ERROR EXCEPTION: " << "Method: " + metoda + " Error: " + query->lastError().databaseText() + " Query: " + query->lastQuery();
    DEBUG_WITH_LINE  << " ERROR EXCEPTION: " << "Method: " + metoda + " Error: " + query->lastError().databaseText() + " Query: " + query->lastQuery() << Qt::endl;
}

CExceptionSql::CExceptionSql(const QString &metoda, const QString &query, const QString &error) : CException(metoda, error)
{
    LOG_SQL << " ERROR EXCEPTION: " << "Method: " + metoda + " Error: " + error + " Query: " + query;
    DEBUG_WITH_LINE << " ERROR EXCEPTION: " << "Method: " + metoda + " Error: " + error + " Query: " + query << Qt::endl;
}

CExceptionSql::~CExceptionSql()
{
}
