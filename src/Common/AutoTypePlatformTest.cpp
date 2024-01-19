#include "AutoTypePlatformTest.h"

CAutoTypePlatformTest::CAutoTypePlatformTest(QObject *parent) : QObject(parent)
{

}

bool CAutoTypePlatformTest::isAvailable()
{
    return true;
}

QString CAutoTypePlatformTest::keyToString(Qt::Key key)
{
    return QString("[Key0x%1]").arg(key, 0, 16);
}

QStringList CAutoTypePlatformTest::windowTitles()
{
    return QStringList();
}

WId CAutoTypePlatformTest::activeWindow()
{
    return 0;
}

QString CAutoTypePlatformTest::activeWindowTitle()
{
    return m_activeWindowTitle;
}

CAutoTypeExecutor* CAutoTypePlatformTest::createExecutor()
{
    return new CAutoTypeExecturorTest(this);
}

void CAutoTypePlatformTest::setActiveWindowTitle(const QString& title)
{
    m_activeWindowTitle = title;
}

QString CAutoTypePlatformTest::actionChars()
{
    return m_actionChars;
}

int CAutoTypePlatformTest::actionCount()
{
    return m_actionList.size();
}

void CAutoTypePlatformTest::clearActions()
{
    qDeleteAll(m_actionList);
    m_actionList.clear();

    m_actionChars.clear();
}

void CAutoTypePlatformTest::addActionChar(CAutoTypeChar* action)
{
    m_actionList.append(action->clone());
    m_actionChars += action->m_character;
}

void CAutoTypePlatformTest::addActionKey(CAutoTypeKey* action)
{
    m_actionList.append(action->clone());
    m_actionChars.append(keyToString(action->m_key));
}

bool CAutoTypePlatformTest::raiseWindow(WId window)
{
    Q_UNUSED(window);

    return false;
}

#ifdef Q_OS_MACOS
bool AutoTypePlatformTest::hideOwnWindow()
{
    return false;
}

bool AutoTypePlatformTest::raiseOwnWindow()
{
    return false;
}
#endif


CAutoTypeExecturorTest::CAutoTypeExecturorTest(CAutoTypePlatformTest* platform)
    : m_platform(platform)
{
}

void CAutoTypeExecturorTest::execChar(CAutoTypeChar* action)
{
    m_platform->addActionChar(action);
}

void CAutoTypeExecturorTest::execKey(CAutoTypeKey* action)
{
    m_platform->addActionKey(action);
}
