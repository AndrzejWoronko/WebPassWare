#include <QObject>
#include <QString>
#include <QtTest>
#include <QApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

#include <memory>

#include "Database.h"
#include "LoginDialog.h"
#include "TestSupport/TestEnvironment.h"

// The login dialog is the only way into the program: it sets the master
// password on the first run and checks it afterwards.
class Test_LoginDialog : public QObject
{
    Q_OBJECT

public:
    explicit Test_LoginDialog(const TestEnvironment &environment) : m_environment(environment) {}

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void test_firstRunSetsPassword();
    void test_correctPasswordOpens();
    void test_wrongPasswordStaysClosed();
    void test_legacyHashIsUpgraded();
    void test_emptyLegacyPasswordRequiresSetup();
    void test_emptyLegacyPasswordCannotBeRehashed();
    void test_cancelledLegacySetupKeepsHash();
    void test_legacySetupRejectsEmptyAndMismatchedPasswords();

private:
    // Fills the fields and presses OK (accept() is the dialog's OK handler).
    static void submit(CLoginDialog &dialog, const QString &passwd, const QString &repeat = QString());
    static QString stored();

    const TestEnvironment &m_environment;
    std::unique_ptr<CDatabase> m_db;
};

void Test_LoginDialog::initTestCase()
{
    QVERIFY(m_environment.isValid());
    QVERIFY(m_environment.ownsDatabasePath(m_environment.databasePath()));
    m_db = std::make_unique<CDatabase>(m_environment.databaseName());
    QCOMPARE(m_db->getBaseNameFullPath(), m_environment.databasePath());
    QVERIFY(m_db->isConnected());
    m_db->setEchoQuery(false);
    m_db->setLogQuery(false);
    m_db->setQueryLog();
}

void Test_LoginDialog::cleanupTestCase()
{
    const QString path = m_environment.databasePath();
    m_db.reset();
    QVERIFY(m_environment.ownsDatabasePath(path));
    QVERIFY(QFile::remove(path));
}

void Test_LoginDialog::init()
{
    QSettings().remove(DATABASE_SETTING_PASSWORD);
}

void Test_LoginDialog::submit(CLoginDialog &dialog, const QString &passwd, const QString &repeat)
{
    dialog.getPasswd()->setText(passwd);
    if (dialog.getPasswd2())
    {
        dialog.getPasswd2()->setText(repeat);
    }
    QMetaObject::invokeMethod(&dialog, "accept");
}

QString Test_LoginDialog::stored()
{
    return QSettings().value(DATABASE_SETTING_PASSWORD).toString();
}

void Test_LoginDialog::test_firstRunSetsPassword()
{
    CLoginDialog dialog(*m_db);
    QVERIFY(dialog.getPasswd2());

    submit(dialog, "sekret", "sekret");
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
    QVERIFY(stored().startsWith("pbkdf2-sha256$"));
    QVERIFY(m_db->checkDatabasePassword("sekret"));
}

void Test_LoginDialog::test_correctPasswordOpens()
{
    m_db->setDatabasePassword("sekret");
    CLoginDialog dialog(*m_db);
    QVERIFY(!dialog.getPasswd2());

    submit(dialog, "sekret");
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
}

void Test_LoginDialog::test_wrongPasswordStaysClosed()
{
    m_db->setDatabasePassword("sekret");
    CLoginDialog dialog(*m_db);

    // The "wrong password" message box is modal; close it once it is up.
    bool messageShown = false;
    QTimer::singleShot(0, [&messageShown]() {
        if (auto *box = qobject_cast<QMessageBox *>(QApplication::activeModalWidget()))
        {
            messageShown = true;
            box->accept();
        }
    });
    submit(dialog, "inne");
    QVERIFY(messageShown);
    QVERIFY(dialog.result() != int(QDialog::Accepted));
}

void Test_LoginDialog::test_legacyHashIsUpgraded()
{
    QSettings().setValue(DATABASE_SETTING_PASSWORD,
                         QString::fromLatin1(QCryptographicHash::hash("stare", QCryptographicHash::Md5).toHex()));
    CLoginDialog dialog(*m_db);
    QVERIFY(!dialog.getPasswd2());

    submit(dialog, "stare");
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
    QVERIFY(stored().startsWith("pbkdf2-sha256$"));
}

static QString emptyLegacyHash()
{
    return QString::fromLatin1(QCryptographicHash::hash(QByteArray(), QCryptographicHash::Md5).toHex());
}

void Test_LoginDialog::test_emptyLegacyPasswordRequiresSetup()
{
    QSettings().setValue(DATABASE_SETTING_PASSWORD, emptyLegacyHash());
    CLoginDialog dialog(*m_db);
    QVERIFY(dialog.getPasswd2());
    QVERIFY(!m_db->hasDatabasePassword());

    submit(dialog, "nowe", "nowe");
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
    QVERIFY(stored().startsWith("pbkdf2-sha256$"));
    QVERIFY(m_db->checkDatabasePassword("nowe"));
    QVERIFY(m_db->checkDatabasePassword("nowe"));
    QVERIFY(!m_db->checkDatabasePassword(QString()));
}

void Test_LoginDialog::test_emptyLegacyPasswordCannotBeRehashed()
{
    const QString legacy = emptyLegacyHash();
    QSettings().setValue(DATABASE_SETTING_PASSWORD, legacy);
    QVERIFY(!m_db->checkDatabasePassword(QString()));
    QCOMPARE(stored(), legacy);
    QVERIFY(!m_db->checkDatabasePassword(QString()));
    QCOMPARE(stored(), legacy);
}

void Test_LoginDialog::test_cancelledLegacySetupKeepsHash()
{
    const QString legacy = emptyLegacyHash();
    QSettings().setValue(DATABASE_SETTING_PASSWORD, legacy);
    {
        CLoginDialog dialog(*m_db);
        QVERIFY(dialog.getPasswd2());
        dialog.reject();
        QCOMPARE(dialog.result(), int(QDialog::Rejected));
    }
    QCOMPARE(stored(), legacy);
    CLoginDialog retry(*m_db);
    QVERIFY(retry.getPasswd2());
}

void Test_LoginDialog::test_legacySetupRejectsEmptyAndMismatchedPasswords()
{
    const QString legacy = emptyLegacyHash();
    QSettings().setValue(DATABASE_SETTING_PASSWORD, legacy);
    CLoginDialog dialog(*m_db);
    QVERIFY(dialog.getPasswd2());
    for (const QString &password : {QString(), QString("nowe")})
    {
        bool messageShown = false;
        QTimer::singleShot(0, [&messageShown]() {
            if (auto *box = qobject_cast<QMessageBox *>(QApplication::activeModalWidget()))
            {
                messageShown = true;
                box->accept();
            }
        });
        submit(dialog, password, QString());
        QVERIFY(messageShown);
        QVERIFY(dialog.result() != int(QDialog::Accepted));
        QCOMPARE(stored(), legacy);
    }
}

int main(int argc, char **argv)
{
    TestEnvironment environment;
    if (!environment.isValid())
    {
        return 1;
    }
    QApplication app(argc, argv);
    Test_LoginDialog test(environment);
    return QTest::qExec(&test, argc, argv);
}

#include "test_logindialog.moc"
