#include <QtTest>
#include <QApplication>
#include <QFile>
#include <QTimer>

#include <memory>

#include "Database.h"
#include "FormSimpleIndexChoiceField.h"
#include "PassEntryDialogController.h"
#include "PassEntryService.h"
#include "PassGroupService.h"
#include "Schema.h"
#include "TestSupport/TestEnvironment.h"

using namespace wpw;

class Test_PassEntryDialog : public QObject
{
    Q_OBJECT

public:
    explicit Test_PassEntryDialog(const TestEnvironment &environment) : m_environment(environment) {}

private Q_SLOTS:
    void initTestCase();
    void cleanup();
    void cleanupTestCase();
    void test_editWithUnavailableGroup_data();
    void test_editWithUnavailableGroup();

private:
    const TestEnvironment &m_environment;
    std::unique_ptr<CDatabase> m_db;
    std::unique_ptr<PassEntryService> m_entries;
    std::unique_ptr<PassGroupService> m_groups;
};

void Test_PassEntryDialog::initTestCase()
{
    QVERIFY(m_environment.isValid());
    m_db = std::make_unique<CDatabase>(m_environment.databaseName());
    QVERIFY(m_environment.ownsDatabasePath(m_db->getBaseNameFullPath()));
    QVERIFY(m_db->isConnected());
    m_db->setEchoQuery(false);
    m_db->setLogQuery(false);
    m_db->setQueryLog();
    QString error;
    QVERIFY2(wpw::Schema::upgrade(*m_db, &error), qPrintable(error));
    m_entries = std::make_unique<PassEntryService>(*m_db);
    m_groups = std::make_unique<PassGroupService>(*m_db);
}

void Test_PassEntryDialog::cleanup()
{
    QVERIFY(m_db && m_environment.ownsDatabasePath(m_db->getBaseNameFullPath()));
    QVERIFY(QSqlQuery(m_db->getDb()).exec("DELETE FROM pass_entry"));
    QVERIFY(QSqlQuery(m_db->getDb()).exec("DELETE FROM pass_group WHERE id_pass_group <> 0"));
}

void Test_PassEntryDialog::cleanupTestCase()
{
    m_entries.reset();
    m_groups.reset();
    m_db.reset();
    QVERIFY(QFile::remove(m_environment.databasePath()));
}

void Test_PassEntryDialog::test_editWithUnavailableGroup_data()
{
    QTest::addColumn<bool>("removeGroup");
    QTest::addColumn<int>("selection");
    QTest::newRow("deleted-preserved") << false << -1;
    QTest::newRow("missing-preserved") << true << -1;
    QTest::newRow("deleted-reassigned") << false << 1;
    QTest::newRow("missing-reassigned") << true << 1;
    QTest::newRow("deleted-reassigned-to-all") << false << 0;
    QTest::newRow("missing-reassigned-to-all") << true << 0;
}

void Test_PassEntryDialog::test_editWithUnavailableGroup()
{
    QFETCH(bool, removeGroup);
    QFETCH(int, selection);

    PassGroup group;
    group.setm_name("Old group");
    const qint64 oldGroupId = m_groups->addObject(&group);
    QVERIFY(oldGroupId > 0);
    PassGroup target;
    target.setm_name("Available group");
    const qint64 targetId = m_groups->addObject(&target);
    QVERIFY(targetId > 0);

    PassEntry entry;
    entry.setm_title("Original title");
    entry.setm_user("user");
    entry.setm_pass("password");
    entry.setm_web_url("https://example.com");
    entry.setm_id_pass_group(oldGroupId);
    const qint64 entryId = m_entries->addObject(&entry);
    QVERIFY(entryId > 0);
    QVERIFY(removeGroup ? m_groups->removeObject(oldGroupId) : m_groups->deleteObject(oldGroupId));

    PassEntryDialogController controller(m_entries.get(), m_groups.get());
    auto *field = dynamic_cast<CFormSimpleIndexChoiceField *>(controller.getDialog()->getFields().value("m_id_pass_group"));
    QVERIFY(field);
    auto *title = controller.getDialog()->getFields().value("m_title");
    QVERIFY(title);
    const qint64 selectedId = selection == 0 ? 0 : targetId;
    int initialIndex = 0;
    QTimer::singleShot(0, controller.getDialog(), [&]() {
        initialIndex = field->currentIndex();
        title->setValue("Changed title");
        if (selection >= 0)
        {
            field->setIdValue(selectedId);
        }
        controller.checkRequiredFields();
    });
    QVERIFY(controller.exec(entryId, "Edit entry"));
    QCOMPARE(initialIndex, -1);
    const auto stored = m_entries->getOwnedObject(entryId);
    QVERIFY(!stored.isNull());
    QCOMPARE(stored->getm_title(), QString("Changed title"));
    QCOMPARE(stored->getm_id_pass_group(), selection < 0 ? oldGroupId : selectedId);
}

int main(int argc, char **argv)
{
    TestEnvironment environment;
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("WebPassWareTests");
    QCoreApplication::setApplicationName("PassEntryDialog");
    Test_PassEntryDialog test(environment);
    return QTest::qExec(&test, argc, argv);
}

#include "test_passentrydialog.moc"
