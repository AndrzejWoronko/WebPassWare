#include <QObject>
#include <QString>
#include <QtTest>
#include <QDir>
#include <QFile>
#include <QUuid>

#include "Database.h"
#include "ExceptionSql.h"
#include "PassEntryService.h"
#include "PassGroupService.h"

#define CLEAR_PTR_LIST_TEST(list) \
    for (int i = 0; i < list.size(); i++) \
        delete list.takeAt(i);

class Test_Service : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void test_servicePrototypeObject();
    void test_passGroupCrud();
    void test_passEntryCrud();
    void test_errorPathSetsError();

private:
    void createTables();
    void clearTables();
};

void Test_Service::initTestCase()
{
    const QString dataPath = QDir::temp().filePath(QString("webpassware-service-test-data-%1").arg(QUuid::createUuid().toString(QUuid::Id128)));
    QDir().mkpath(dataPath);
    qputenv("XDG_DATA_HOME", QFile::encodeName(dataPath));

    SingletonDatabase::getInstance();
    DB.setEchoQuery(false);
    DB.setLogQuery(false);
    DB.setEchoError(false);
    DB.setLogError(false);
    DB.setQueryLog();

    createTables();
}

void Test_Service::cleanupTestCase()
{
    QFile file(DB.getBaseNameFullPath());
    file.remove();
}

void Test_Service::cleanup()
{
    clearTables();
}

void Test_Service::createTables()
{
    PassGroup passGroup;
    PassEntry passEntry;

    QVERIFY(passGroup.createTable());
    QVERIFY(passEntry.createTable());
}

void Test_Service::clearTables()
{
    DB.getDb().exec(QString("DELETE FROM pass_entry;"));
    DB.getDb().exec(QString("DELETE FROM pass_group;"));
}

void Test_Service::test_servicePrototypeObject()
{
    PassGroupService service;

    QVERIFY(service.getObject() != nullptr);
    QCOMPARE(service.getObject()->getTableName(), QString("pass_group"));
}

void Test_Service::test_passGroupCrud()
{
    PassGroupService service;
    PassGroup group;
    group.setm_name("Prywatne");

    const qint64 id = service.addObject(&group);
    QVERIFY(id > 0);
    QCOMPARE(service.getInsertedId(), id);
    QVERIFY(service.getError().isEmpty());

    QScopedPointer<PassGroup> found(service.getObject(id));
    QVERIFY(!found.isNull());
    QCOMPARE(found->getm_name(), QString("Prywatne"));

    found->setm_name("Praca");
    QVERIFY(service.editObject(found.data()));

    QList<PassGroup*> groups = service.getObjects(QString("id_pass_group"));
    QCOMPARE(groups.size(), 1);
    QCOMPARE(groups.first()->getm_name(), QString("Praca"));
    CLEAR_PTR_LIST_TEST(groups)

    const QStringList groupNames = service.getGroupNameList();
    QCOMPARE(groupNames, QStringList() << "Praca");

    QVERIFY(service.deleteObject(id));
    QScopedPointer<PassGroup> deleted(service.getObject(id));
    QVERIFY(!deleted.isNull());
    QCOMPARE(deleted->getdeleted(), true);

    QVERIFY(service.removeObject(id));
    QScopedPointer<PassGroup> removed(service.getObject(id));
    QVERIFY(removed.isNull());
}

void Test_Service::test_passEntryCrud()
{
    PassGroupService groupService;
    PassGroup group;
    group.setm_name("Loginy");
    const qint64 groupId = groupService.addObject(&group);
    QVERIFY(groupId > 0);

    PassEntryService service;
    PassEntry entry;
    entry.setm_title("Email");
    entry.setm_user("user@example.com");
    entry.setm_pass("secret");
    entry.setm_web_url("https://example.com");
    entry.setm_desc("Opis");
    entry.setm_id_pass_group(groupId);

    const qint64 id = service.addObject(&entry);
    QVERIFY(id > 0);
    QVERIFY(service.getError().isEmpty());

    QScopedPointer<PassEntry> found(service.getObject(id));
    QVERIFY(!found.isNull());
    QCOMPARE(found->getm_title(), QString("Email"));
    QCOMPARE(found->getm_id_pass_group(), groupId);

    found->setm_title("Email updated");
    QVERIFY(service.editObject(found.data()));

    QList<PassEntry*> byTitle = service.getObjectsBy("m_title", QVariant("Email updated"));
    QCOMPARE(byTitle.size(), 1);
    QCOMPARE(byTitle.first()->getId(), id);
    CLEAR_PTR_LIST_TEST(byTitle)

    QHash<QString, QVariant> params;
    params.insert("m_user", QVariant("user@example.com"));
    QList<PassEntry*> byParams = service.getObjectsBy(params);
    QCOMPARE(byParams.size(), 1);
    QCOMPARE(byParams.first()->getId(), id);
    CLEAR_PTR_LIST_TEST(byParams)

    QVERIFY(service.deleteObject(id));
    QScopedPointer<PassEntry> deleted(service.getObject(id));
    QVERIFY(!deleted.isNull());
    QCOMPARE(deleted->getdeleted(), true);

    QVERIFY(service.removeObject(id));
    QScopedPointer<PassEntry> removed(service.getObject(id));
    QVERIFY(removed.isNull());
}

void Test_Service::test_errorPathSetsError()
{
    PassEntryService service;

    QList<PassEntry*> list = service.getObjectsBy("missing_column", QVariant("value"));
    QCOMPARE(list.size(), 0);
    QVERIFY(!service.getError().isEmpty());
    CLEAR_PTR_LIST_TEST(list)

    list = service.getObjects();
    QVERIFY(service.getError().isEmpty());
    CLEAR_PTR_LIST_TEST(list)
}

QTEST_MAIN(Test_Service)

#include "test_service.moc"
