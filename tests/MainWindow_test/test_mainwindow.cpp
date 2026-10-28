#include <QObject>
#include <QString>
#include <QtTest>
#include <QAbstractButton>
#include <QClipboard>
#include <QComboBox>
#include <QDialog>
#include <QMessageBox>
#include <QTemporaryDir>
#include <QTimer>
#include <QTreeView>

#include <functional>
#include <memory>

#include "Application.h"
#include "AutoType.h"
#include "AutoTypePlatformTest.h"
#include "Database.h"
#include "FormSimpleIndexChoiceField.h"
#include "PassEntryService.h"
#include "PassGroupService.h"
#include "Schema.h"
#include "SettingsWidget.h"
#include "TableView.h"
#include "TestSupport/TestEnvironment.h"
#include "WebPassWareMainWindow.h"

using namespace wpw;

static QString settingsOrganization()
{
#ifdef Q_OS_DARWIN
    return QString(APP_ORGANIZATIO_DOMAIN);
#else
    return QString(APP_ORGANIZATION_NAME);
#endif
}

// Main window offscreen on a temporary database. Modal dialogs and message
// boxes are closed by a timer that counts them.
class Test_MainWindow : public QObject
{
    Q_OBJECT

public:
    Test_MainWindow(CApplication &app, const TestEnvironment &environment) : m_app(app), m_environment(environment) {}

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void test_noNullActions();
    void test_storageIsIsolated();
    void test_savedThemeIsRestoredAtStartup();
    void test_qtTextsArePolish();
    void test_shortcutsAreUnique();
    void test_insertOpensOneDialog();
    void test_deletedGroupLeavesList();
    void test_formDialogsDoNotPileUp();
    void test_viewsFollowServiceChanges();
    void test_copyToClipboard();
    void test_deleteEntry();
    void test_editDialogShowsEntryGroup();
    void test_settingsDialogOpensAndCloses();
    void test_closeDialogsOpenAndClose();
    void test_settingsPagesFollowList();

private:
    // Closes every modal widget that appears; message boxes get \a answer.
    void autoClose(QMessageBox::StandardButton answer = QMessageBox::No);
    QAction *action(const QString &name) const;
    bool selectGroup(const QString &name);

    CApplication &m_app;
    const TestEnvironment &m_environment;
    std::unique_ptr<CDatabase> m_db;
    std::unique_ptr<PassEntryService> m_entries;
    std::unique_ptr<PassGroupService> m_groups;
    std::unique_ptr<CAutoType> m_autoType;
    std::unique_ptr<CWebPassWareMainWindow> m_window;
    QTimer m_closer;
    QStringList m_closed;
    QMessageBox::StandardButton m_answer = QMessageBox::No;
    // Called with each modal widget before it is closed.
    std::function<void(QWidget *)> m_inspect;
};

void Test_MainWindow::initTestCase()
{
    QVERIFY(m_environment.isValid());
    m_db = std::make_unique<CDatabase>(m_environment.databaseName());
    QVERIFY(m_environment.ownsDatabasePath(m_db->getBaseNameFullPath()));
    m_db->setEchoQuery(false);
    m_db->setLogQuery(false);
    m_db->setEchoError(false);
    m_db->setLogError(false);
    m_db->setQueryLog();
    QString error;
    QVERIFY2(wpw::Schema::upgrade(*m_db, &error), qPrintable(error));
    m_entries = std::make_unique<PassEntryService>(*m_db);
    m_autoType = std::make_unique<CAutoType>(std::make_unique<CAutoTypePlatformTest>());
    m_groups = std::make_unique<PassGroupService>(*m_db);

    m_closer.setInterval(20);
    connect(&m_closer, &QTimer::timeout, this, [this]() {
        QWidget *modal = QApplication::activeModalWidget();
        if (!modal)
        {
            return;
        }
        m_closed.append(QString::fromLatin1(modal->metaObject()->className()));
        if (m_inspect)
        {
            m_inspect(modal);
        }
        if (auto *box = qobject_cast<QMessageBox *>(modal))
        {
            QAbstractButton *button = box->button(m_answer);
            if (button)
            {
                button->click();
                return;
            }
        }
        if (auto *dialog = qobject_cast<QDialog *>(modal))
        {
            dialog->reject();
        }
    });
}

void Test_MainWindow::init()
{
    // Qt warns about a null action added to a menu or tool bar.
    QTest::failOnWarning(QRegularExpression("null action"));
    PassGroup group;
    group.setm_name("Bank");
    QVERIFY(m_groups->addObject(&group) > 0);
    PassEntry entry;
    entry.setm_title("Konto");
    entry.setm_user("jan");
    entry.setm_pass("sekret");
    entry.setm_id_pass_group(group.getId());
    QVERIFY(m_entries->addObject(&entry) > 0);

    m_window = std::make_unique<CWebPassWareMainWindow>(m_app, *m_db, m_entries.get(), m_groups.get(), *m_autoType);
    m_window->show();
    QVERIFY(QTest::qWaitForWindowExposed(m_window.get()));
    m_closed.clear();
}

void Test_MainWindow::cleanup()
{
    m_closer.stop();
    m_inspect = nullptr;
    m_window.reset();
    QVERIFY(m_db && m_environment.ownsDatabasePath(m_db->getBaseNameFullPath()));
    QSqlQuery(m_db->getDb()).exec("DELETE FROM pass_entry");
    QSqlQuery(m_db->getDb()).exec("DELETE FROM pass_group WHERE id_pass_group <> 0");
}

void Test_MainWindow::cleanupTestCase()
{
    m_entries.reset();
    m_groups.reset();
    m_db.reset();
    QVERIFY(QFile::remove(m_environment.databasePath()));
}

void Test_MainWindow::test_storageIsIsolated()
{
    QVERIFY(QStandardPaths::isTestModeEnabled());
    QVERIFY(m_environment.ownsDatabasePath(m_db->getBaseNameFullPath()));
    QVERIFY(m_db->getBaseName() != QString(DB_NAME));
    QCOMPARE(QSettings().format(), QSettings::IniFormat);
    QCOMPARE(QDir(m_environment.settingsPath()).relativeFilePath(QSettings().fileName()),
             QString("%1/%2.ini").arg(settingsOrganization(), APP_NAME));
}

void Test_MainWindow::test_savedThemeIsRestoredAtStartup()
{
    QFile theme(QString("%1/dark").arg(STYLE_PATH));
    QVERIFY(theme.open(QIODevice::ReadOnly));
    const QString expected = QString::fromUtf8(theme.readAll());
    QVERIFY(!expected.isEmpty());
    QCOMPARE(m_app.styleSheet(), expected);
}

void Test_MainWindow::autoClose(QMessageBox::StandardButton answer)
{
    m_answer = answer;
    m_closer.start();
}

QAction *Test_MainWindow::action(const QString &name) const
{
    return m_window->findChild<QAction *>(name);
}

bool Test_MainWindow::selectGroup(const QString &name)
{
    auto *tree = m_window->findChild<QTreeView *>();
    const QModelIndexList found = tree->model()->match(tree->model()->index(0, 0), Qt::DisplayRole, name, 1,
                                                       Qt::MatchExactly | Qt::MatchRecursive);
    for (int column = 0; found.isEmpty() && column < tree->model()->columnCount(); ++column)
    {
        const QModelIndexList inColumn = tree->model()->match(tree->model()->index(0, column), Qt::DisplayRole, name, 1, Qt::MatchExactly);
        if (!inColumn.isEmpty())
        {
            tree->setCurrentIndex(inColumn.first());
            return true;
        }
    }
    if (found.isEmpty())
    {
        return false;
    }
    tree->setCurrentIndex(found.first());
    return true;
}

void Test_MainWindow::test_noNullActions()
{
    // Construction in init() would have failed on the warning.
    QVERIFY(action("ACTION_EXIT"));
}

void Test_MainWindow::test_shortcutsAreUnique()
{
    QHash<QString, QString> owners;
    const QList<QAction *> actions = m_window->findChildren<QAction *>();
    for (QAction *a : actions)
    {
        const QString keys = a->shortcut().toString();
        if (keys.isEmpty())
        {
            continue;
        }
        QVERIFY2(!owners.contains(keys), qPrintable(QString("%1 on %2 and %3").arg(keys, owners.value(keys), a->objectName())));
        owners.insert(keys, a->objectName());
    }
}

void Test_MainWindow::test_insertOpensOneDialog()
{
    auto *table = m_window->findChild<CTableView *>();
    table->setFocus();
    autoClose();
    QTest::keyClick(table, Qt::Key_Insert);
    QTRY_VERIFY(QApplication::activeModalWidget() == nullptr);
    QTest::qWait(200);
    QCOMPARE(m_closed.count(QString("wpw::PassEntryDialog")), 1);
}

void Test_MainWindow::test_deletedGroupLeavesList()
{
    auto *tree = m_window->findChild<QTreeView *>();
    const int before = tree->model()->rowCount();
    QVERIFY(selectGroup("Bank"));
    autoClose(QMessageBox::Yes);
    action("ACTION_DEL_PASS_GROUP")->trigger();
    QCOMPARE(tree->model()->rowCount(), before - 1);
}

void Test_MainWindow::test_formDialogsDoNotPileUp()
{
    autoClose();
    action("ACTION_ADD_PASS_GROUP")->trigger();
    action("ACTION_ADD_PASS_GROUP")->trigger();
    action("ACTION_ADD_PASS_ENTRY")->trigger();
    QCOMPARE(m_closed.count(), 3);
    QCOMPARE(m_window->findChildren<QDialog *>().size(), 0);
}

void Test_MainWindow::test_viewsFollowServiceChanges()
{
    auto *table = m_window->findChild<CTableView *>();
    auto *tree = m_window->findChild<QTreeView *>();
    const int entries = table->model()->rowCount();
    const int groups = tree->model()->rowCount();

    PassEntry entry;
    entry.setm_title("Nowy");
    QVERIFY(m_entries->addObject(&entry) > 0);
    QCOMPARE(table->model()->rowCount(), entries + 1);

    PassGroup group;
    group.setm_name("Nowa");
    QVERIFY(m_groups->addObject(&group) > 0);
    QCOMPARE(tree->model()->rowCount(), groups + 1);
}

void Test_MainWindow::test_copyToClipboard()
{
    auto *table = m_window->findChild<CTableView *>();
    QVERIFY(table->model()->rowCount() > 0);
    table->setCurrentIndex(table->model()->index(0, 0));

    action("ACTION_COPY_PASS_ENTRY_PASSWORD")->trigger();
    QCOMPARE(QApplication::clipboard()->text(), QString("sekret"));
    action("ACTION_COPY_PASS_ENTRY_USER")->trigger();
    QCOMPARE(QApplication::clipboard()->text(), QString("jan"));

    // Nothing selected: nothing copied, no message.
    QApplication::clipboard()->setText("bez zmian");
    table->setCurrentIndex(QModelIndex());
    autoClose();
    action("ACTION_COPY_PASS_ENTRY_PASSWORD")->trigger();
    QCOMPARE(QApplication::clipboard()->text(), QString("bez zmian"));
    QVERIFY(m_closed.isEmpty());
}

void Test_MainWindow::test_deleteEntry()
{
    auto *table = m_window->findChild<CTableView *>();
    const int before = table->model()->rowCount();
    QVERIFY(before > 0);
    table->setCurrentIndex(table->model()->index(0, 0));
    autoClose(QMessageBox::Yes);
    action("ACTION_DEL_PASS_ENTRY")->trigger();
    QCOMPARE(m_closed.size(), 1);
    QCOMPARE(table->model()->rowCount(), before - 1);
}

void Test_MainWindow::test_editDialogShowsEntryGroup()
{
    // Ids with a hole: the combo position no longer equals the group id.
    PassGroup removed;
    removed.setm_name("Usunięta");
    QVERIFY(m_groups->addObject(&removed) > 0);
    PassGroup target;
    target.setm_name("Cel");
    QVERIFY(m_groups->addObject(&target) > 0);
    QVERIFY(m_groups->removeObject(removed.getId()));
    PassEntry entry;
    entry.setm_title("W celu");
    entry.setm_id_pass_group(target.getId());
    QVERIFY(m_entries->addObject(&entry) > 0);

    auto *table = m_window->findChild<CTableView *>();
    const QModelIndexList rows = table->model()->match(table->model()->index(0, 0), Qt::DisplayRole, QVariant(entry.getId()), 1, Qt::MatchExactly);
    QVERIFY(!rows.isEmpty());
    table->setCurrentIndex(rows.first());

    QString shownGroup;
    m_inspect = [&shownGroup](QWidget *modal) {
        const QList<QComboBox *> combos = modal->findChildren<QComboBox *>();
        for (QComboBox *combo : combos)
        {
            if (dynamic_cast<CFormSimpleIndexChoiceField *>(combo))
            {
                shownGroup = combo->currentText();
            }
        }
    };
    autoClose();
    action("ACTION_EDIT_PASS_ENTRY")->trigger();
    QCOMPARE(shownGroup, QString("Cel"));
}

void Test_MainWindow::test_qtTextsArePolish()
{
    // Qt's own texts (dialog buttons) follow the Polish user interface.
    QCOMPARE(QCoreApplication::translate("QPlatformTheme", "&Yes"), QString("&Tak"));
}

void Test_MainWindow::test_settingsDialogOpensAndCloses()
{
    // The pages belong to their controllers; opening twice must not crash.
    autoClose();
    action("ACTION_SETTINGS_DIALOG")->trigger();
    action("ACTION_SETTINGS_DIALOG")->trigger();
    QCOMPARE(m_closed.size(), 2);
    QCOMPARE(m_window->findChildren<QDialog *>().size(), 0);
}

void Test_MainWindow::test_closeDialogsOpenAndClose()
{
    // About and the password generator are close-only dialogs.
    autoClose();
    action("ACTION_ABOUT")->trigger();
    action("ACTION_GENERATOR_DIALOG")->trigger();
    QCOMPARE(m_closed.size(), 2);
    QCOMPARE(m_window->findChildren<QDialog *>().size(), 0);
}

void Test_MainWindow::test_settingsPagesFollowList()
{
    SettingsWidget settings(m_app, *m_db);
    QCOMPARE(settings.getList()->count(), 2);
    QCOMPARE(settings.getLayout()->count(), settings.getList()->count());
    settings.getList()->setCurrentRow(1);
    QCOMPARE(settings.getLayout()->currentIndex(), 1);
    settings.getList()->setCurrentRow(0);
    QCOMPARE(settings.getLayout()->currentIndex(), 0);
}

int main(int argc, char **argv)
{
    TestEnvironment environment;
    if (!environment.isValid())
    {
        return 1;
    }
    // Seed the actual application identity without hiding a missing startup setup.
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, settingsOrganization(), APP_NAME);
        settings.setValue(SETTINGS_GUI_STYLE, int(CStyle::DARK));
        settings.sync();
        if (settings.status() != QSettings::NoError)
        {
            return 1;
        }
    }
    CApplication app(argc, argv);
    Test_MainWindow test(app, environment);
    return QTest::qExec(&test, argc, argv);
}

#include "test_mainwindow.moc"
