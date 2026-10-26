#include <QtTest>

#include <memory>

#include "AutoType.h"
#include "AutoTypePlatformTest.h"
#ifdef Q_OS_LINUX
#include "AutoTypePlatformLinux.h"
#endif

class Test_AutoType : public QObject
{
    Q_OBJECT

private:
    CAutoTypePlatformTest* m_platform = nullptr;
    std::unique_ptr<CAutoType> m_autoType;

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
    void test_syntaxErrorTypesNothing();
    void test_linuxPlatformOnX11();
};

void Test_AutoType::initTestCase()
{
    auto platform = std::make_unique<CAutoTypePlatformTest>();
    m_platform = platform.get();
    m_autoType = std::make_unique<CAutoType>(std::move(platform));
}

void Test_AutoType::init()
{
    m_platform->clearActions();
}

void Test_AutoType::test_usernameAndPassword()
{
    Entry entry("alice", "s3cr3t");
    m_autoType->performAutoType(&entry);

    QString expected = "alice" + m_platform->keyToString(Qt::Key_Tab)
                      + "s3cr3t" + m_platform->keyToString(Qt::Key_Enter);
    QCOMPARE(m_platform->actionChars(), expected);
}

void Test_AutoType::test_usernameOnly()
{
    Entry entry("alice", "");
    m_autoType->performAutoType(&entry);

    QString expected = "alice" + m_platform->keyToString(Qt::Key_Enter);
    QCOMPARE(m_platform->actionChars(), expected);
}

void Test_AutoType::test_passwordOnly()
{
    Entry entry("", "s3cr3t");
    m_autoType->performAutoType(&entry);

    QString expected = "s3cr3t" + m_platform->keyToString(Qt::Key_Enter);
    QCOMPARE(m_platform->actionChars(), expected);
}

void Test_AutoType::test_emptyEntry()
{
    Entry entry("", "");
    m_autoType->performAutoType(&entry);

    QCOMPARE(m_platform->actionCount(), 0);
}

void Test_AutoType::test_repeatedKey()
{
    Entry entry("alice", "s3cr3t");
    m_autoType->performAutoType(&entry, nullptr, "{TAB 3}");

    QString expected = m_platform->keyToString(Qt::Key_Tab).repeated(3);
    QCOMPARE(m_platform->actionChars(), expected);
}

void Test_AutoType::test_functionKey()
{
    Entry entry("alice", "s3cr3t");
    m_autoType->performAutoType(&entry, nullptr, "{F5}");

    QCOMPARE(m_platform->actionChars(), m_platform->keyToString(Qt::Key_F5));
}

void Test_AutoType::test_delay()
{
    Entry entry("alice", "s3cr3t");
    m_autoType->performAutoType(&entry, nullptr, "a{DELAY 10}b");

    QCOMPARE(m_platform->actionChars(), QString("ab"));
}

void Test_AutoType::test_escapeBraces()
{
    Entry entry("alice", "s3cr3t");
    m_autoType->performAutoType(&entry, nullptr, "{{}a{}}b");

    QCOMPARE(m_platform->actionChars(), QString("{a}b"));
}

void Test_AutoType::test_setDelay()
{
    Entry entry("alice", "s3cr3t");
    m_autoType->performAutoType(&entry, nullptr, "{DELAY=0}ab");

    QCOMPARE(m_platform->actionChars(), QString("ab"));
}

void Test_AutoType::test_syntaxErrorTypesNothing()
{
    Entry entry("alice", "secret");
    m_autoType->performAutoType(&entry, nullptr, "ab}c");
    QCOMPARE(m_platform->actionChars(), QString());
}

void Test_AutoType::test_linuxPlatformOnX11()
{
#ifdef Q_OS_LINUX
    // The real platform on the test's X11 display: no typing, only queries.
    CAutoTypePlatformLinux platform;
    if (!platform.isAvailable())
    {
        QSKIP("No X11 display with the XTEST extension.");
    }
    // Queries over the X11 connection must not crash (BadWindow is trapped).
    platform.activeWindow();
    platform.activeWindowTitle();
    platform.windowTitles();
    platform.unload();
    QVERIFY(!platform.isAvailable());
#else
    QSKIP("Linux only.");
#endif
}

QTEST_MAIN(Test_AutoType)

#include "test_autotype.moc"
