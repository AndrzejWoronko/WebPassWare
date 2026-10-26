#include "AutoType.h"
#include <QApplication>
#include <QRegularExpression>

#include "AutoTypePlatform.h"
#include "AutoTypePlatformTest.h"
#include "Tools.h"

#ifdef Q_OS_WIN  //Windows

#include "AutoTypePlatformWindows.h"

#elif defined(Q_OS_MACOS) //Macos

#include "AutoTypePlatformMacos.h"

#else            //Linux

#include "AutoTypePlatformLinux.h"

#endif

CAutoType::CAutoType(std::unique_ptr<CAutoTypePlatform> platform, QObject* parent)
    : QObject(parent)
    , m_inAutoType(false)
    , m_currentGlobalKey(static_cast<Qt::Key>(0))
    , m_currentGlobalModifiers(QFlag(0))
    , m_platform_intrerface(std::move(platform))
    , m_executor(nullptr)
    , m_windowFromGlobal(0)
{
    if (m_platform_intrerface)
      {
        m_executor = m_platform_intrerface->createExecutor();
      }
}

CAutoType::~CAutoType()
{
   // The executor uses the platform.
   safe_delete(m_executor);
   m_platform_intrerface.reset();
}

std::unique_ptr<CAutoTypePlatform> CAutoType::createPlatform()
{
#ifdef Q_OS_WIN  //Windows
    return std::make_unique<CAutoTypePlatformWindows>();
#elif defined(Q_OS_MACOS) //Macos
    return std::make_unique<CAutoTypePlatformMacos>();
#else            //Linux
    return std::make_unique<CAutoTypePlatformLinux>();
#endif
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
        // The actions parsed before the syntax error.
        CLEAR_PTR_LIST(actions);
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

    Tools::wait(10);

    for (auto action : std::as_const(actions))
      {
        if (m_platform_intrerface->activeWindow() != window)
          {
            qWarning("Active window changed, interrupting auto-type.");
            break;
          }

        action->accept(m_executor);
        Tools::wait(m_executor->delay());
      }
    CLEAR_PTR_LIST(actions);
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

    for (auto entry : std::as_const(entryList))
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
            for (auto entry : std::as_const(entryList))
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
        m_platform_intrerface.reset();
    }
}

bool CAutoType::registerGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers)
{
  Q_UNUSED(key);
  Q_UNUSED(modifiers);
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
    Q_UNUSED(event)
/*
    if (!m_platform_intrerface) {
        return -1;
    }

    return m_platform_intrerface->platformEventFilter(event);
*/
    return -1;
}

bool CAutoType::parseActions(const QString& sequence, const Entry* entry, QList<CAutoTypeAction*>& actions)
{
    QString tmpl;
    bool inTmpl = false;

    for (auto ch : std::as_const(sequence))
      {
        if (inTmpl)
          {
            if (ch == '{')
              {
                // "{{}" escape sequence -> literal '{'
                if (tmpl.isEmpty())
                  {
                    tmpl += ch;
                  }
                else
                  {
                    DEBUG_WITH_LINE << "Syntax error in auto-type sequence.";
                    return false;
                  }
              }
            else if (ch == '}')
              {
                // "{}}" escape sequence -> literal '}': the first '}' starts
                // the placeholder name, the second one closes it.
                if (tmpl.isEmpty())
                  {
                    tmpl += ch;
                  }
                else
                  {
                    actions.append(this->createActionFromTemplate(tmpl, entry));
                    inTmpl = false;
                    tmpl.clear();
                  }
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

    // {DELAY=X} sets a persistent delay between subsequent actions,
    // as opposed to {DELAY X} which inserts a single pause.
    static const QRegularExpression setDelayRegEx(QRegularExpression::anchoredPattern("delay=(\\d+)"));
    const QRegularExpressionMatch setDelayMatch = setDelayRegEx.match(tmplName);
    if (setDelayMatch.hasMatch())
      {
        int delayMs = setDelayMatch.captured(1).toInt();
        if (delayMs <= 10000)
          {
            list.append(new CAutoTypeSetDelay(delayMs));
          }
        return list;
      }

    static const QRegularExpression repeatRegEx(QRegularExpression::anchoredPattern("(.+) (\\d+)"));
    const QRegularExpressionMatch repeatMatch = repeatRegEx.match(tmplName);
    if (repeatMatch.hasMatch())
      {
        tmplName = repeatMatch.captured(1);
        num = repeatMatch.captured(2).toInt();

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
        static const QRegularExpression fnRegexp(QRegularExpression::anchoredPattern("f(\\d+)"),
                                                 QRegularExpression::CaseInsensitiveOption);
        const QRegularExpressionMatch fnMatch = fnRegexp.match(tmplName);
        if (fnMatch.hasMatch()) {
            int fnNo = fnMatch.captured(1).toInt();
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
        for (auto ch : std::as_const(resolved))
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
            sequence = "{USERNAME}{TAB}{DELAY 500}{PASSWORD}{ENTER}";
          }
    }
    return sequence;
}
