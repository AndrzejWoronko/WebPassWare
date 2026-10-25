#include "TransactionGuard.h"

#include <QSqlError>
#include <QSqlQuery>

#include <utility>

namespace wpw {

TransactionGuard::TransactionGuard(QSqlDatabase db)
    : m_db(std::move(db))
{
    QSqlQuery query(m_db);
    m_active = query.exec(QStringLiteral("BEGIN IMMEDIATE"));
    if (!m_active)
    {
        m_error = query.lastError().text();
    }
}

TransactionGuard::~TransactionGuard()
{
    if (m_active)
    {
        QSqlQuery query(m_db);
        query.exec(QStringLiteral("ROLLBACK"));
    }
}

bool TransactionGuard::commit()
{
    if (!m_active)
    {
        return false;
    }
    QSqlQuery query(m_db);
    if (!query.exec(QStringLiteral("COMMIT")))
    {
        m_error = query.lastError().text();
        return false;
    }
    m_active = false;
    return true;
}

} // namespace wpw
