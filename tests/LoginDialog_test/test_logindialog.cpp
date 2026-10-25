#include <QObject>
#include <QString>
#include <QtTest>
#include <QApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QTemporaryDir>
#include <QTimer>

#include <memory>

#include "Database.h"
#include "LoginDialog.h"

// The login dialog is the only way into the program: it sets the master
// password on the first run and checks it afterwards.
class Test_LoginDialog : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void test_firstRunSetsPassword();
    void test_correctPasswordOpens();
    void test_wrongPasswordStaysClosed();
    void test_legacyHashIsUpgraded();

private:
    // Fills the fields and presses OK (accept() is the dialog's OK handler).
    static void submit(CLoginDialog &dialog, const QString &passwd, const QString &repeat = QString());
    static QString stored();

    QTemporaryDir m_dir;
    std::unique_ptr<CDatabase> m_db;
};

void Test_LoginDialog::initTestCase()
{
    QVERIFY(m_dir.isValid());
    qputenv("XDG_DATA_HOME", QFile::encodeName(m_dir.filePath("data")));
    qputenv("XDG_CONFIG_HOME", QFile::encodeName(m_dir.filePath("config")));
    m_db = std::make_unique<CDatabase>();
    m_db->setEchoQuery(false);
    m_db->setLogQuery(false);
}

void Test_LoginDialog::cleanupTestCase()
{
    m_db.reset();
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

QTEST_MAIN(Test_LoginDialog)

#include "test_logindialog.moc"
