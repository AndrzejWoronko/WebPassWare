#include <QObject>
#include <QString>
#include <QtTest>
#include <QDir>
#include <QFile>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QTemporaryDir>

#include <memory>

#include "Database.h"
#include "PassEntryService.h"
#include "PassGroupService.h"
#include "Schema.h"

using namespace wpw;

// Every start brings the database file to wpw::Schema::kVersion: an empty
// file gets the tables, a file of the Qt 5 version (user_version 0) is
// migrated, a file of a newer version is left alone.
class Test_Schema : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanup();
    void test_emptyDatabase();
    void test_legacyDatabase();
    void test_upgradeIsIdempotent();
    void test_newerDatabaseIsRefused();
    void test_softDeleteWritesOne();
    void test_backupBeforeMigration();
    void test_missingColumnIsAdded();

private:
    // Opens a fresh copy of the legacy fixture, or an empty file.
    void open(bool legacy);
    int version();
    QVariant rawValue(const QString &sql);

    QTemporaryDir m_dir;
    QString m_path;
    std::unique_ptr<CDatabase> m_db;
};

void Test_Schema::initTestCase()
{
    QVERIFY(m_dir.isValid());
    qputenv("XDG_DATA_HOME", QFile::encodeName(m_dir.path()));
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QVERIFY(QDir().mkpath(dir));
    m_path = QDir(dir).filePath(DB_NAME);
}

void Test_Schema::cleanup()
{
    m_db.reset();
    QFile::remove(m_path);
    QFile::remove(m_path + ".bak-v0");
}

void Test_Schema::open(bool legacy)
{
    m_db.reset();
    QFile::remove(m_path);
    if (legacy)
    {
        QVERIFY(QFile::copy(QString(FIXTURES_DIR) + "/legacy_qt5.db", m_path));
        QVERIFY(QFile::setPermissions(m_path, QFile::ReadOwner | QFile::WriteOwner));
    }
    m_db = std::make_unique<CDatabase>();
    m_db->setEchoQuery(false);
    m_db->setLogQuery(false);
    QVERIFY(m_db->isConnected());
}

int Test_Schema::version()
{
    return rawValue("PRAGMA user_version").toInt();
}

QVariant Test_Schema::rawValue(const QString &sql)
{
    QSqlQuery query(m_db->getDb());
    if (!query.exec(sql) || !query.next())
    {
        return QVariant();
    }
    return query.value(0);
}

void Test_Schema::test_emptyDatabase()
{
    open(false);
    QString error;
    QVERIFY2(wpw::Schema::upgrade(*m_db, &error), qPrintable(error));
    QCOMPARE(version(), wpw::Schema::kVersion);
    QVERIFY(m_db->checkTableInDatabase("pass_group"));
    QVERIFY(m_db->checkTableInDatabase("pass_entry"));

    PassGroupService groups(*m_db);
    PassGroupService::OwnedObject all = groups.getOwnedObject(0);
    QVERIFY(!all.isNull());
    QCOMPARE(all->getm_name(), QString::fromUtf8("Wszystkie grupy"));
}

void Test_Schema::test_legacyDatabase()
{
    open(true);
    QCOMPARE(version(), 0);
    QString error;
    QVERIFY2(wpw::Schema::upgrade(*m_db, &error), qPrintable(error));
    QCOMPARE(version(), wpw::Schema::kVersion);

    // Soft-deleted rows held the text 'true'; now 1 like everywhere else.
    QCOMPARE(rawValue("SELECT COUNT(*) FROM pass_group WHERE deleted = 'true'").toInt(), 0);
    QCOMPARE(rawValue("SELECT deleted FROM pass_group WHERE id_pass_group = 2").toInt(), 1);
    QCOMPARE(rawValue("SELECT deleted FROM pass_entry WHERE id_pass_entry = 3").toInt(), 1);
    QCOMPARE(rawValue("SELECT deleted FROM pass_entry WHERE id_pass_entry = 1").toInt(), 0);

    PassEntryService entries(*m_db);
    PassEntryService::OwnedObject entry = entries.getOwnedObject(1);
    QVERIFY(!entry.isNull());
    QCOMPARE(entry->getm_pass(), QString::fromUtf8("Zażółć;gęślą\"jaźń"));
    QCOMPARE(entries.getOwnedObject(3)->getdeleted(), true);

    QScopedPointer<CSqlModel> model(entries.getSqlModel());
    model->refresh();
    QCOMPARE(model->rowCount(), 2);
}

void Test_Schema::test_upgradeIsIdempotent()
{
    open(true);
    QString error;
    QVERIFY(wpw::Schema::upgrade(*m_db, &error));
    QVERIFY2(wpw::Schema::upgrade(*m_db, &error), qPrintable(error));
    QCOMPARE(version(), wpw::Schema::kVersion);
    QCOMPARE(rawValue("SELECT COUNT(*) FROM pass_group").toInt(), 3);
}

void Test_Schema::test_newerDatabaseIsRefused()
{
    open(true);
    QSqlQuery query(m_db->getDb());
    QVERIFY(query.exec(QString("PRAGMA user_version = %1").arg(wpw::Schema::kVersion + 1)));

    QString error;
    QVERIFY(!wpw::Schema::upgrade(*m_db, &error));
    QVERIFY(!error.isEmpty());
    QCOMPARE(version(), wpw::Schema::kVersion + 1);
    QCOMPARE(rawValue("SELECT deleted FROM pass_group WHERE id_pass_group = 2").toString(), QString("true"));
}

void Test_Schema::test_softDeleteWritesOne()
{
    open(false);
    QString error;
    QVERIFY(wpw::Schema::upgrade(*m_db, &error));
    PassGroupService groups(*m_db);
    PassGroup group;
    group.setm_name("Do usunięcia");
    const qint64 id = groups.addObject(&group);
    QVERIFY(id > 0);
    QVERIFY(groups.deleteObject(id));
    QCOMPARE(rawValue(QString("SELECT typeof(deleted) || ':' || deleted FROM pass_group WHERE id_pass_group = %1").arg(id)).toString(),
             QString("integer:1"));
}

void Test_Schema::test_backupBeforeMigration()
{
    open(false);
    QString error;
    QVERIFY(wpw::Schema::upgrade(*m_db, &error));
    // A new, empty file has nothing worth a copy.
    QVERIFY(!QFile::exists(m_path + ".bak-v0"));

    open(true);
    QVERIFY2(wpw::Schema::upgrade(*m_db, &error), qPrintable(error));
    QFile backup(m_path + ".bak-v0");
    QVERIFY(backup.open(QIODevice::ReadOnly));
    QFile fixture(QString(FIXTURES_DIR) + "/legacy_qt5.db");
    QVERIFY(fixture.open(QIODevice::ReadOnly));
    QVERIFY(backup.readAll() == fixture.readAll());
}

void Test_Schema::test_missingColumnIsAdded()
{
    // A file whose table lacks a column of the model (an older layout).
    open(false);
    QSqlQuery query(m_db->getDb());
    QVERIFY(query.exec("CREATE TABLE pass_group( id_pass_group INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                       "deleted BOOL NOT NULL DEFAULT FALSE, timestp TIMESTAMP NOT NULL)"));
    QVERIFY(query.exec("INSERT INTO pass_group (id_pass_group, deleted, timestp) VALUES (5, 0, '2026-01-01')"));

    QString error;
    QVERIFY2(wpw::Schema::upgrade(*m_db, &error), qPrintable(error));
    QVERIFY(m_db->getTableFields("pass_group").contains("m_name"));
    QCOMPARE(rawValue("SELECT COUNT(*) FROM sqlite_master WHERE name = 'pass_group_m_name_idx'").toInt(), 1);
    QCOMPARE(rawValue("SELECT COUNT(*) FROM pass_group WHERE id_pass_group = 5").toInt(), 1);
}

QTEST_GUILESS_MAIN(Test_Schema)

#include "test_schema.moc"
