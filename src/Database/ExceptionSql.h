#ifndef CEXCEPTIONSQL_H
#define CEXCEPTIONSQL_H

#include "Global.h"
#include "SqlQuery.h"
#include "Exception.h"
#include "Logger.h"

namespace wpw {

class CExceptionSql : public CException
{
private:

public:

    CExceptionSql(const QString &metoda, CSqlQuery query);
    CExceptionSql(const QString &metoda, CSqlQuery *query);
    CExceptionSql(const QString &metoda, const QString &query, const QString &error);
    ~CExceptionSql();
};

} // namespace wpw

#endif // CEXCEPTIONSQL_H
