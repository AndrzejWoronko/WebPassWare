#ifndef TRANSACTIONGUARD_H
#define TRANSACTIONGUARD_H

#include <QSqlDatabase>
#include <QString>

namespace wpw {

// BEGIN IMMEDIATE on construction (takes the write lock at once), ROLLBACK
// on destruction unless commit() succeeded. SQLite has no nested
// transactions: a guard opened inside another one is inactive.
class TransactionGuard
{
public:
    explicit TransactionGuard(QSqlDatabase db);
    ~TransactionGuard();
    TransactionGuard(const TransactionGuard &) = delete;
    TransactionGuard &operator=(const TransactionGuard &) = delete;

    bool isActive() const { return m_active; }
    bool commit();
    QString error() const { return m_error; }

private:
    QSqlDatabase m_db;
    bool m_active = false;
    QString m_error;
};

} // namespace wpw

#endif // TRANSACTIONGUARD_H
