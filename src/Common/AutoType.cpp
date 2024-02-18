#include "AutoType.h"
#include <QApplication>

#include "AutoTypePlatform.h"
#include "AutoTypePlatformTest.h"

#ifdef Q_OS_WIN  //Windows

#include "AutoTypePlatformWindows.h"

#elif Q_OS_MACOS //Macos

#include "AutoTypePlatformMacos.h"

#else            //Linux

#include "AutoTypePlatformLinux.h"

#endif

CAutoType* CAutoType::m_instance = nullptr;

CAutoType::CAutoType(QObject* parent, bool test)
    : QObject(parent)
    , m_inAutoType(false)
    , m_currentGlobalKey(static_cast<Qt::Key>(0))
    , m_currentGlobalModifiers(QFlag(0))
    , m_platform_intrerface(nullptr)
    , m_executor(nullptr)
    , m_windowFromGlobal(0)
{
    if (test)
      m_platform_intrerface = new CAutoTypePlatformTest(this);
    else
      {
#ifdef Q_OS_WIN  //Windows
      m_platform_intrerface = new CAutoTypePlatformWindows(this);
#elif Q_OS_MACOS //Macos
      m_platform_intrerface = new CAutoTypePlatformMacos(this);
#else            //Linux
      //m_platform_intrerface = new CAutoTypePlatformLinux(this);
#endif
      }
}

CAutoType::~CAutoType()
{
   safe_delete(m_platform_intrerface)
   safe_delete(m_executor)
}


CAutoType* CAutoType::instance()
{
    if (!m_instance)
      {
        m_instance = new CAutoType(qApp);
      }

    return m_instance;
}

void CAutoType::createTestInstance()
{
    Q_ASSERT(!m_instance);

    m_instance = new CAutoType(qApp, true);
}

QStringList CAutoType::windowTitles()
{
    if (!m_platform_intrerface)
      {
        return QStringList();
      }

    return m_platform_intrerface->windowTitles();
}

void CAutoType::performAutoType(const Entry* entry, QWidget* hideWindow, const QString& customSequence, WId window)
{
    if (m_inAutoType || !m_platform_intrerface)
      {
        return;
      }
    m_inAutoType = true;

    QString sequence;
    if (customSequence.isEmpty())
      {
        sequence = this->autoTypeSequence(entry);
      }
    else
      {
        sequence = customSequence;
      }

    QList<CAutoTypeAction*> actions;

    if (!parseActions(sequence, entry, actions))
      {
        m_inAutoType = false; // TODO: make this automatic
        return;
      }

    if (hideWindow)
      {
        hideWindow->showMinimized();
      }
    // TODO
    //Tools::wait(m_platform_intrerface->initialTimeout());

    if (!window)
      {
        window = m_platform_intrerface->activeWindow();
      }

    QCoreApplication::processEvents(QEventLoop::AllEvents, 10);

    Q_FOREACH(auto action, actions)
      {
        if (m_platform_intrerface->activeWindow() != window)
          {
            qWarning("Active window changed, interrupting auto-type.");
            break;
          }

        action->accept(m_executor);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
      }
    CLEAR_PTR_LIST(actions)
    m_inAutoType = false;
}

void CAutoType::performGlobalAutoType(const QList<Entry*>& entryList)
{
    if (m_inAutoType || !m_platform_intrerface)
      {
        return;
      }

    QString windowTitle = m_platform_intrerface->activeWindowTitle();

    if (windowTitle.isEmpty())
      {
        return;
      }

    m_inAutoType = true;

    QHash<Entry*, QString> sequenceHash;

    Q_FOREACH(auto entry, entryList)
      {
          QString sequence = this->autoTypeSequence(entry);
          if (!sequence.isEmpty())
            {
               sequenceHash.insert(entry, sequence);
            }
      }

    if (!entryList.isEmpty())
      {
        if (entryList.size() == 1)
          {
            m_inAutoType = false;
            performAutoType(entryList.first(), nullptr, sequenceHash.value(entryList.first()));
          }
        else
          {
            m_windowFromGlobal = m_platform_intrerface->activeWindow();
            Q_FOREACH(auto entry, entryList)
              {
                performAutoTypeFromGlobal(entry, sequenceHash.value(entry));
              }
          }
      }
}

void CAutoType::performAutoTypeFromGlobal(Entry* entry, const QString& sequence)
{
    Q_ASSERT(m_inAutoType);

    m_platform_intrerface->raiseWindow(m_windowFromGlobal);

    m_inAutoType = false;
    this->performAutoType(entry, nullptr, sequence, m_windowFromGlobal);
}

void CAutoType::resetInAutoType()
{
    Q_ASSERT(m_inAutoType);

    m_inAutoType = false;
}

void CAutoType::unloadPlugin()
{
    if (m_executor) {
        delete m_executor;
        m_executor = nullptr;
    }

    if (m_platform_intrerface) {
        m_platform_intrerface->unload();
        m_platform_intrerface = nullptr;
    }
}

bool CAutoType::registerGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers)
{
/*   compile comment

    Q_ASSERT(key);
    Q_ASSERT(modifiers);

    if (!m_platform_intrerface) {
        return false;
    }

    if (key != m_currentGlobalKey || modifiers != m_currentGlobalModifiers) {
        if (m_currentGlobalKey && m_currentGlobalModifiers) {
            m_platform_intrerface->unregisterGlobalShortcut(m_currentGlobalKey, m_currentGlobalModifiers);
        }

        if (m_platform_intrerface->registerGlobalShortcut(key, modifiers)) {
            m_currentGlobalKey = key;
            m_currentGlobalModifiers = modifiers;
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return true;
    }
*/
    return true;
}

void CAutoType::unregisterGlobalShortcut()
{
/* compile comment
    if (m_platform_intrerface && m_currentGlobalKey && m_currentGlobalModifiers)
      {
        m_platform_intrerface->unregisterGlobalShortcut(m_currentGlobalKey, m_currentGlobalModifiers);
      }
*/
}

int CAutoType::callEventFilter(void* event)
{
/*
    if (!m_platform_intrerface) {
        return -1;
    }

    return m_platform_intrerface->platformEventFilter(event);
*/
}

bool CAutoType::parseActions(const QString& sequence, const Entry* entry, QList<CAutoTypeAction*>& actions)
{
    QString tmpl;
    bool inTmpl = false;

    Q_FOREACH(auto ch, sequence)
      {
        // TODO: implement support for {{}, {}} and {DELAY=X}

        if (inTmpl)
          {
            if (ch == '{')
              {
                DEBUG_WITH_LINE << "Syntax error in auto-type sequence.";
                return false;
              }
            else if (ch == '}')
              {
                actions.append(this->createActionFromTemplate(tmpl, entry));
                inTmpl = false;
                tmpl.clear();
              }
            else
              {
                tmpl += ch;
              }
        }
        else if (ch == '{')
          {
            inTmpl = true;
          }
        else if (ch == '}')
          {
            DEBUG_WITH_LINE << "Syntax error in auto-type sequence.";
            return false;
          }
        else
          {
            actions.append(new CAutoTypeChar(ch));
          }
      }
    return true;
}

QList<CAutoTypeAction*> CAutoType::createActionFromTemplate(const QString& tmpl, const Entry* entry)
{
    QString tmplName = tmpl.toLower();
    int num = -1;
    QList<CAutoTypeAction*> list;

    QRegExp repeatRegEx("(.+) (\\d+)", Qt::CaseSensitive, QRegExp::RegExp2);
    if (repeatRegEx.exactMatch(tmplName))
      {
        tmplName = repeatRegEx.cap(1);
        num = repeatRegEx.cap(2).toInt();

        if (num == 0)
          {
            return list;
          }
        // some safety checks
        else if (tmplName == "delay")
          {
            if (num > 10000) {
                return list;
            }
          }
        else if (num > 100)
          {
            return list;
          }
      }

    if (tmplName == "tab")
      {
        list.append(new CAutoTypeKey(Qt::Key_Tab));
      }
    else if (tmplName == "enter")
      {
        list.append(new CAutoTypeKey(Qt::Key_Enter));
      }
    else if (tmplName == "up")
      {
        list.append(new CAutoTypeKey(Qt::Key_Up));
      }
    else if (tmplName == "down")
      {
        list.append(new CAutoTypeKey(Qt::Key_Down));
      }
    else if (tmplName == "left")
      {
        list.append(new CAutoTypeKey(Qt::Key_Left));
      }
    else if (tmplName == "right")
      {
        list.append(new CAutoTypeKey(Qt::Key_Right));
      }
    else if (tmplName == "insert" || tmplName == "ins")
      {
        list.append(new CAutoTypeKey(Qt::Key_Insert));
      }
    else if (tmplName == "delete" || tmplName == "del")
      {
        list.append(new CAutoTypeKey(Qt::Key_Delete));
      }
    else if (tmplName == "home")
      {
        list.append(new CAutoTypeKey(Qt::Key_Home));
      }
    else if (tmplName == "end")
      {
        list.append(new CAutoTypeKey(Qt::Key_End));
      }
    else if (tmplName == "pgup")
      {
        list.append(new CAutoTypeKey(Qt::Key_PageUp));
      }
    else if (tmplName == "pgdown")
    {
        list.append(new CAutoTypeKey(Qt::Key_PageDown));
    }
    else if (tmplName == "backspace" || tmplName == "bs" || tmplName == "bksp")
    {
        list.append(new CAutoTypeKey(Qt::Key_Backspace));
    }
    else if (tmplName == "break")
      {
        list.append(new CAutoTypeKey(Qt::Key_Pause));
      }
    else if (tmplName == "capslock")
      {
        list.append(new CAutoTypeKey(Qt::Key_CapsLock));
      }
    else if (tmplName == "esc")
      {
        list.append(new CAutoTypeKey(Qt::Key_Escape));
      }
    else if (tmplName == "help") {
        list.append(new CAutoTypeKey(Qt::Key_Help));
    }
    else if (tmplName == "numlock")
      {
        list.append(new CAutoTypeKey(Qt::Key_NumLock));
      }
    else if (tmplName == "ptrsc")
      {
        list.append(new CAutoTypeKey(Qt::Key_Print));
      }
    else if (tmplName == "scolllock")
      {
        list.append(new CAutoTypeKey(Qt::Key_ScrollLock));
      }
    // Qt doesn't know about keypad keys so use the normal ones instead
    else if (tmplName == "add" || tmplName == "+")
      {
        list.append(new CAutoTypeChar('+'));
      }
    else if (tmplName == "subtract")
      {
        list.append(new CAutoTypeChar('-'));
      }
    else if (tmplName == "multiply")
      {
        list.append(new CAutoTypeChar('*'));
      }
    else if (tmplName == "divide")
      {
        list.append(new CAutoTypeChar('/'));
      }
    else if (tmplName == "^")
      {
        list.append(new CAutoTypeChar('^'));
      }
    else if (tmplName == "%")
      {
        list.append(new CAutoTypeChar('%'));
      }
    else if (tmplName == "~")
      {
        list.append(new CAutoTypeChar('~'));
      }
    else if (tmplName == "(")
      {
        list.append(new CAutoTypeChar('('));
      }
    else if (tmplName == ")")
      {
        list.append(new CAutoTypeChar(')'));
      }
    else if (tmplName == "{")
      {
        list.append(new CAutoTypeChar('{'));
      }
    else if (tmplName == "}")
      {
        list.append(new CAutoTypeChar('}'));
      }
    else
      {
        QRegExp fnRegexp("f(\\d+)", Qt::CaseInsensitive, QRegExp::RegExp2);
        if (fnRegexp.exactMatch(tmplName)) {
            int fnNo = fnRegexp.cap(1).toInt();
            if (fnNo >= 1 && fnNo <= 16) {
                list.append(new CAutoTypeKey(static_cast<Qt::Key>(Qt::Key_F1 - 1 + fnNo)));
            }
        }
      }

    if (!list.isEmpty())
      {
        for (int i = 1; i < num; i++)
          {
            list.append(list.at(0)->clone());
          }
        return list;
      }


    if (tmplName == "delay" && num > 0)
      {
        list.append(new CAutoTypeDelay(num));
      }

    if (!list.isEmpty())
      {
        return list;
      }

    const QString placeholder = QString("{%1}").arg(tmplName);
    QString resolved = placeholder;
    resolved.replace("{USERNAME}", entry->first, Qt::CaseInsensitive);
    resolved.replace("{PASSWORD}", entry->second, Qt::CaseInsensitive);

    if (placeholder != resolved) //Nastąpiła zamiana
      {
        Q_FOREACH(auto ch, resolved)
          {
            if (ch == '\n')
              {
                list.append(new CAutoTypeKey(Qt::Key_Enter));
              }
            else if (ch == '\t')
              {
                list.append(new CAutoTypeKey(Qt::Key_Tab));
              }
            else
              {
                list.append(new CAutoTypeChar(ch));
              }
          }
      }
    return list;
}

QString CAutoType::autoTypeSequence(const Entry* entry)
{
    QString sequence;
    sequence.clear();

    if ((!entry->first.isEmpty() || !entry->second.isEmpty()))
      {
        if (entry->first.isEmpty())
          {
            sequence = "{PASSWORD}{ENTER}";
          }
        else if (entry->second.isEmpty())
          {
            sequence = "{USERNAME}{ENTER}";
          }
        else
          {
            sequence = "{USERNAME}{TAB}{PASSWORD}{ENTER}";
          }
    }
    return sequence;
}
