#include <QObject>
#include <QString>
#include <QtTest>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTemporaryDir>

#include <memory>

#include "Database.h"
#include "PassEntryService.h"
#include "PassGroupService.h"
#include "SqlModel.h"

// Reads tests/fixtures/legacy_qt5.db, a database written by the Qt 5 build
// (schema from "webpassware -c", data through the services; see
// tests/fixtures/README.md). Users' existing files look like this one, so the
// program must keep reading them unchanged.
class Test_LegacyDb : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test_groups();
    void test_entries();
    void test_listModelsSkipDeletedRows();
    void test_listModelAlignsNumbersRight();
    void test_listModelHeadersAndExport();

private:
    QTemporaryDir m_dataDir;
    std::unique_ptr<CDatabase> m_db;
};

void Test_LegacyDb::initTestCase()
{
    QVERIFY(m_dataDir.isValid());
    qputenv("XDG_DATA_HOME", QFile::encodeName(m_dataDir.path()));

    // The database file must be in place before CDatabase opens it.
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QVERIFY(QDir().mkpath(dir));
    const QString target = QDir(dir).filePath(DB_NAME);
    QVERIFY(QFile::copy(QString(FIXTURES_DIR) + "/legacy_qt5.db", target));
    QVERIFY(QFile::setPermissions(target, QFile::ReadOwner | QFile::WriteOwner));

    m_db = std::make_unique<CDatabase>();
    m_db->setEchoQuery(false);
    m_db->setLogQuery(false);
    m_db->setEchoError(false);
    m_db->setLogError(false);
    QVERIFY(m_db->isConnected());
    QCOMPARE(m_db->getBaseNameFullPath(), target);
}

void Test_LegacyDb::test_groups()
{
    PassGroupService service(*m_db);

    PassGroupService::OwnedObjectList groups = service.getOwnedObjects(QString("id_pass_group"));
    QCOMPARE(groups.size(), 3);
    QCOMPARE(groups.at(0)->getId(), qint64(0));
    QCOMPARE(groups.at(0)->getm_name(), QString::fromUtf8("Wszystkie grupy"));
    QCOMPARE(groups.at(1)->getm_name(), QString::fromUtf8("Bankowość"));
    QCOMPARE(groups.at(1)->getdeleted(), false);
    QCOMPARE(groups.at(2)->getm_name(), QString::fromUtf8("Usunięta grupa"));
    // Soft delete stored the text 'true'.
    QCOMPARE(groups.at(2)->getdeleted(), true);

    QCOMPARE(service.getGroupNameList(),
             QStringList() << QString::fromUtf8("Wszystkie grupy") << QString::fromUtf8("Bankowość")
                           << QString::fromUtf8("Usunięta grupa"));
}

void Test_LegacyDb::test_entries()
{
    PassEntryService service(*m_db);

    PassEntryService::OwnedObject entry = service.getOwnedObject(1);
    QVERIFY(!entry.isNull());
    QCOMPARE(entry->getm_title(), QString::fromUtf8("Konto główne"));
    QCOMPARE(entry->getm_user(), QString("jan.kowalski"));
    QCOMPARE(entry->getm_pass(), QString::fromUtf8("Zażółć;gęślą\"jaźń"));
    QCOMPARE(entry->getm_web_url(), QString("https://bank.example.pl"));
    QCOMPARE(entry->getm_desc(), QString::fromUtf8("Opis z ąćęłńóśźż"));
    QCOMPARE(entry->getm_id_pass_group(), qint64(1));
    QCOMPARE(entry->getdeleted(), false);
    QVERIFY(entry->gettimestp().isValid());

    PassEntryService::OwnedObject removed = service.getOwnedObject(3);
    QVERIFY(!removed.isNull());
    QCOMPARE(removed->getdeleted(), true);

    PassEntryService::OwnedObjectList all = service.getOwnedObjects(QString("id_pass_entry"));
    QCOMPARE(all.size(), 3);
}

void Test_LegacyDb::test_listModelsSkipDeletedRows()
{
    QScopedPointer<CSqlModel> groups(PassGroupService(*m_db).getSqlModel());
    groups->refresh();
    QCOMPARE(groups->rowCount(), 2);

    QScopedPointer<CSqlModel> entries(PassEntryService(*m_db).getSqlModel());
    entries->refresh();
    QCOMPARE(entries->rowCount(), 2);
}

void Test_LegacyDb::test_listModelAlignsNumbersRight()
{
    QScopedPointer<CSqlModel> entries(PassEntryService(*m_db).getSqlModel());
    entries->refresh();
    const int idColumn = entries->record().indexOf("id_pass_entry");
    const int titleColumn = entries->record().indexOf("m_title");
    QVERIFY(idColumn >= 0);
    QVERIFY(titleColumn >= 0);

    const QVariant idAlignment = entries->data(entries->index(0, idColumn), Qt::TextAlignmentRole);
    const QVariant titleAlignment = entries->data(entries->index(0, titleColumn), Qt::TextAlignmentRole);
    QCOMPARE(idAlignment.toInt(), int(Qt::AlignRight | Qt::AlignVCenter));
    QCOMPARE(titleAlignment.toInt(), int(Qt::AlignLeft | Qt::AlignVCenter));
}

void Test_LegacyDb::test_listModelHeadersAndExport()
{
    QScopedPointer<CSqlModel> entries(PassEntryService(*m_db).getSqlModel());
    entries->refresh();
    const int title = entries->columnIndex("m_title");
    const int group = entries->columnIndex("pass_group_m_name");
    QVERIFY(title >= 0 && group >= 0);
    QCOMPARE(entries->getFieldNameByColumn(title), QString("m_title"));
    QCOMPARE(entries->headerData(title, Qt::Horizontal).toString(), QString::fromUtf8("Tytuł"));
    QCOMPARE(entries->headerData(title, Qt::Horizontal, Qt::ToolTipRole).toString(), QString::fromUtf8("Tytuł rekordu"));
    QCOMPARE(entries->rawData(entries->index(0, title)).toString(), QString::fromUtf8("Konto główne"));
    // The group name comes from the sub-select in the field description.
    QCOMPARE(entries->data(entries->index(0, group)).toString(), QString::fromUtf8("Bankowość"));
    QCOMPARE(entries->data(entries->index(1, group)).toString(), QString::fromUtf8("Wszystkie grupy"));

    const QVector<QVariant> header = entries->toVariantRowHeader();
    QCOMPARE(header.size(), entries->columnCount());
    QCOMPARE(header.at(title).toString(), QString::fromUtf8("Tytuł"));
    const QList<QVector<QVariant>> rows = entries->toVariantRowListData();
    QCOMPARE(rows.size(), 2);
    QCOMPARE(rows.at(1).at(title).toString(), QString("Poczta"));

    entries->setSort(title, Qt::DescendingOrder);
    entries->setWhere(QString("m_id_pass_group = 1"));
    entries->refresh();
    QCOMPARE(entries->rowCount(), 1);
}

QTEST_MAIN(Test_LegacyDb)

#include "test_legacydb.moc"
