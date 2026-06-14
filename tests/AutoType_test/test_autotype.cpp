#include <QtTest>

#include "AutoType.h"
#include "AutoTypePlatformTest.h"

class Test_AutoType : public QObject
{
    Q_OBJECT

private:
    CAutoTypePlatformTest* m_platform;

private slots:
    void initTestCase();
    void init();

    void test_usernameAndPassword();
    void test_usernameOnly();
    void test_passwordOnly();
    void test_emptyEntry();
    void test_repeatedKey();
    void test_functionKey();
    void test_delay();
    void test_escapeBraces();
    void test_setDelay();
};

void Test_AutoType::initTestCase()
{
    CAutoType::createTestInstance();
    m_platform = CAutoType::instance()->findChild<CAutoTypePlatformTest*>();
    QVERIFY(m_platform != nullptr);
}

void Test_AutoType::init()
{
    m_platform->clearActions();
}

void Test_AutoType::test_usernameAndPassword()
{
    Entry entry("alice", "s3cr3t");
    CAutoType::instance()->performAutoType(&entry);

    QString expected = "alice" + m_platform->keyToString(Qt::Key_Tab)
                      + "s3cr3t" + m_platform->keyToString(Qt::Key_Enter);
    QCOMPARE(m_platform->actionChars(), expected);
}

void Test_AutoType::test_usernameOnly()
{
    Entry entry("alice", "");
    CAutoType::instance()->performAutoType(&entry);

    QString expected = "alice" + m_platform->keyToString(Qt::Key_Enter);
    QCOMPARE(m_platform->actionChars(), expected);
}

void Test_AutoType::test_passwordOnly()
{
    Entry entry("", "s3cr3t");
    CAutoType::instance()->performAutoType(&entry);

    QString expected = "s3cr3t" + m_platform->keyToString(Qt::Key_Enter);
    QCOMPARE(m_platform->actionChars(), expected);
}

void Test_AutoType::test_emptyEntry()
{
    Entry entry("", "");
    CAutoType::instance()->performAutoType(&entry);

    QCOMPARE(m_platform->actionCount(), 0);
}

void Test_AutoType::test_repeatedKey()
{
    Entry entry("alice", "s3cr3t");
    CAutoType::instance()->performAutoType(&entry, nullptr, "{TAB 3}");

    QString expected = m_platform->keyToString(Qt::Key_Tab).repeated(3);
    QCOMPARE(m_platform->actionChars(), expected);
}

void Test_AutoType::test_functionKey()
{
    Entry entry("alice", "s3cr3t");
    CAutoType::instance()->performAutoType(&entry, nullptr, "{F5}");

    QCOMPARE(m_platform->actionChars(), m_platform->keyToString(Qt::Key_F5));
}

void Test_AutoType::test_delay()
{
    Entry entry("alice", "s3cr3t");
    CAutoType::instance()->performAutoType(&entry, nullptr, "a{DELAY 10}b");

    QCOMPARE(m_platform->actionChars(), QString("ab"));
}

void Test_AutoType::test_escapeBraces()
{
    Entry entry("alice", "s3cr3t");
    CAutoType::instance()->performAutoType(&entry, nullptr, "{{}a{}}b");

    QCOMPARE(m_platform->actionChars(), QString("{a}b"));
}

void Test_AutoType::test_setDelay()
{
    Entry entry("alice", "s3cr3t");
    CAutoType::instance()->performAutoType(&entry, nullptr, "{DELAY=0}ab");

    QCOMPARE(m_platform->actionChars(), QString("ab"));
}

QTEST_MAIN(Test_AutoType)

#include "test_autotype.moc"
