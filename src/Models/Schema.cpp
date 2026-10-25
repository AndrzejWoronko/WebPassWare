#include "Schema.h"

#include <QCoreApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

#include "Database.h"
#include "ModelTableCheck.h"
#include "TransactionGuard.h"

namespace wpw::Schema {

namespace {

// Migration N brings the data from version N-1 to N. Append only: a
// released step never changes.
const QList<QStringList> &migrations()
{
    static const QList<QStringList> steps{
        // 1: soft delete stored the text 'true'; store 1 like the other rows.
        {
            QStringLiteral("UPDATE pass_group SET deleted = 1 WHERE deleted = 'true'"),
            QStringLiteral("UPDATE pass_group SET deleted = 0 WHERE deleted = 'false'"),
            QStringLiteral("UPDATE pass_entry SET deleted = 1 WHERE deleted = 'true'"),
            QStringLiteral("UPDATE pass_entry SET deleted = 0 WHERE deleted = 'false'"),
        },
    };
    return steps;
}

void setError(QString *error, const QString &text)
{
    if (error)
    {
        *error = text;
    }
}

} // namespace

int version(CDatabase &db)
{
    QSqlQuery query(db.getDb());
    if (!query.exec(QStringLiteral("PRAGMA user_version")) || !query.next())
    {
        return -1;
    }
    return query.value(0).toInt();
}

bool upgrade(CDatabase &db, QString *error)
{
    const int current = version(db);
    if (current < 0)
    {
        setError(error, QCoreApplication::translate("Schema", "Nie można odczytać wersji bazy danych: %1")
                            .arg(db.getDb().lastError().text()));
        return false;
    }
    if (current > kVersion)
    {
        setError(error, QCoreApplication::translate("Schema", "Baza danych %1 pochodzi z nowszej wersji programu (wersja %2, obsługiwana %3).")
                            .arg(db.getBaseNameFullPath())
                            .arg(current)
                            .arg(kVersion));
        return false;
    }
    if (current == kVersion)
    {
        return true;
    }

    // Tables, columns and indexes the models describe (an empty file gets
    // everything, an old one what it lacks).
    ModelTableCheck::checkAllTables(db);

    for (int step = current + 1; step <= kVersion; ++step)
    {
        TransactionGuard tx(db.getDb());
        if (!tx.isActive())
        {
            setError(error, tx.error());
            return false;
        }
        QSqlQuery query(db.getDb());
        for (const QString &statement : migrations().at(step - 1))
        {
            if (!query.exec(statement))
            {
                setError(error, QCoreApplication::translate("Schema", "Migracja %1 nie powiodła się: %2")
                                    .arg(step)
                                    .arg(query.lastError().text()));
                return false;
            }
        }
        // PRAGMA takes no bound values; step is a number of ours.
        if (!query.exec(QStringLiteral("PRAGMA user_version = %1").arg(step)) || !tx.commit())
        {
            setError(error, QCoreApplication::translate("Schema", "Migracja %1 nie powiodła się: %2")
                                .arg(step)
                                .arg(query.lastError().text() + tx.error()));
            return false;
        }
    }
    return true;
}

} // namespace wpw::Schema
