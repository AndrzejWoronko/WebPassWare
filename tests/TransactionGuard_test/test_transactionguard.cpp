#include <QObject>
#include <QString>
#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryDir>

#include "TransactionGuard.h"

using namespace wpw;

class Test_TransactionGuard : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void test_commitKeepsChanges();
    void test_destructorRollsBack();
    void test_nestedGuardIsInactive();

private:
    int rows();

    QTemporaryDir m_dir;
    QSqlDatabase m_db;
};

void Test_TransactionGuard::initTestCase()
{
    QVERIFY(m_dir.isValid());
    m_db = QSqlDatabase::addDatabase("QSQLITE", "guard_test");
    m_db.setDatabaseName(m_dir.filePath("guard.db"));
    QVERIFY(m_db.open());
    QVERIFY(QSqlQuery(m_db).exec("CREATE TABLE t (v INTEGER)"));
}

void Test_TransactionGuard::cleanupTestCase()
{
    m_db.close();
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase("guard_test");
}

void Test_TransactionGuard::init()
{
    QVERIFY(QSqlQuery(m_db).exec("DELETE FROM t"));
}

int Test_TransactionGuard::rows()
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT COUNT(*) FROM t") || !query.next())
    {
        return -1;
    }
    return query.value(0).toInt();
}

void Test_TransactionGuard::test_commitKeepsChanges()
{
    {
        wpw::TransactionGuard tx(m_db);
        QVERIFY(tx.isActive());
        QVERIFY(QSqlQuery(m_db).exec("INSERT INTO t VALUES (1)"));
        QVERIFY(tx.commit());
        QVERIFY(!tx.isActive());
    }
    QCOMPARE(rows(), 1);
}

void Test_TransactionGuard::test_destructorRollsBack()
{
    {
        wpw::TransactionGuard tx(m_db);
        QVERIFY(tx.isActive());
        QVERIFY(QSqlQuery(m_db).exec("INSERT INTO t VALUES (1)"));
    }
    QCOMPARE(rows(), 0);
}

void Test_TransactionGuard::test_nestedGuardIsInactive()
{
    wpw::TransactionGuard outer(m_db);
    QVERIFY(outer.isActive());
    wpw::TransactionGuard inner(m_db);
    QVERIFY(!inner.isActive());
    QVERIFY(!inner.error().isEmpty());
    QVERIFY(!inner.commit());
}

QTEST_GUILESS_MAIN(Test_TransactionGuard)

#include "test_transactionguard.moc"
