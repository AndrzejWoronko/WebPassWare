#include <QObject>
#include <QString>
#include <QtTest>
#include <QCryptographicHash>

#include "MasterPassword.h"

class Test_MasterPassword : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_hashFormat();
    void test_verify();
    void test_saltDiffers();
    void test_nonLatin1Characters();
    void test_legacyMd5();
    void test_malformed();
};

static QString legacyMd5(const QString &password)
{
    // What WebPassWare stored before: unsalted MD5 of the Latin-1 bytes.
    return QString::fromLatin1(QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5).toHex());
}

void Test_MasterPassword::test_hashFormat()
{
    const QStringList parts = wpw::MasterPassword::hash("sekret").split('$');
    QCOMPARE(parts.size(), 4);
    QCOMPARE(parts.at(0), QString("pbkdf2-sha256"));
    QCOMPARE(parts.at(1).toInt(), wpw::MasterPassword::kIterations);
    QCOMPARE(QByteArray::fromBase64(parts.at(2).toLatin1()).size(), 16);
    QCOMPARE(QByteArray::fromBase64(parts.at(3).toLatin1()).size(), 32);
}

void Test_MasterPassword::test_verify()
{
    const QString stored = wpw::MasterPassword::hash("sekret");
    QVERIFY(wpw::MasterPassword::verify("sekret", stored));
    QVERIFY(!wpw::MasterPassword::verify("Sekret", stored));
    QVERIFY(!wpw::MasterPassword::verify(QString(), stored));
    QVERIFY(!wpw::MasterPassword::needsRehash(stored));
}

void Test_MasterPassword::test_saltDiffers()
{
    QVERIFY(wpw::MasterPassword::hash("sekret") != wpw::MasterPassword::hash("sekret"));
}

void Test_MasterPassword::test_nonLatin1Characters()
{
    // toLatin1() turned both into "za??", the old hash could not tell them apart.
    const QString stored = wpw::MasterPassword::hash(QString::fromUtf8("zaćź"));
    QVERIFY(wpw::MasterPassword::verify(QString::fromUtf8("zaćź"), stored));
    QVERIFY(!wpw::MasterPassword::verify(QString::fromUtf8("zaśż"), stored));
}

void Test_MasterPassword::test_legacyMd5()
{
    const QString stored = legacyMd5("sekret");
    QVERIFY(wpw::MasterPassword::verify("sekret", stored));
    QVERIFY(!wpw::MasterPassword::verify("inne", stored));
    QVERIFY(wpw::MasterPassword::needsRehash(stored));
    // Recognise the legacy empty password so the login can require setup.
    QVERIFY(wpw::MasterPassword::verify(QString(), legacyMd5(QString())));
}

void Test_MasterPassword::test_malformed()
{
    QVERIFY(!wpw::MasterPassword::verify("sekret", QString()));
    QVERIFY(!wpw::MasterPassword::verify("sekret", "pbkdf2-sha256$x$AAAA$AAAA"));
    QVERIFY(!wpw::MasterPassword::verify("sekret", "pbkdf2-sha256$100000$$"));
    QVERIFY(!wpw::MasterPassword::verify("sekret", "something"));
    QVERIFY(!wpw::MasterPassword::needsRehash(QString()));
}

QTEST_MAIN(Test_MasterPassword)

#include "test_masterpassword.moc"
