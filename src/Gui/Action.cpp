#include "Action.h"
#include "Style.h"

CAction::CAction(QObject *parent) : QAction(parent)
{
    m_actionName = QString();
    m_iconName = QString();
    m_text = QString();
}

CAction::CAction(const QString &text, const QString &shortCut, const QString &name, QObject *parent) : QAction(text, parent)
{
    m_actionName = name;
    m_iconName = QString();
    m_text = text;

    if (!shortCut.isEmpty())
        setShortcut( QKeySequence(shortCut));
    if (!name.isEmpty())
       {
        m_actionName = name;
        this->setObjectName(m_actionName);
       }
    else
    {
    if (!text.isEmpty())
       {
         m_actionName = QString("ACTION_%1").arg(text).toUpper();
         m_actionName.remove("&");
         this->setObjectName(m_actionName);
       }
    }
}

CAction::CAction(const QString &text, const QString &iconName, const QString &iconText, const QString &shortCut, const QString &name, QObject *parent) : QAction(text, parent)
{
    m_actionName = name;
    m_iconName = iconName;
    m_text = text;

    if (CStyle::iconFromStyleExist(m_iconName))
        this->setIcon(CStyle::iconFromStyle(m_iconName));
    else
        this->setIcon(QIcon::fromTheme(m_iconName));

    this->setIconText(iconText);

    if (!shortCut.isEmpty())
        this->setShortcut( QKeySequence(shortCut));
    if (!name.isEmpty())
       {
        m_actionName = name;
        this->setObjectName(m_actionName);
       }
    else
       {
        if (!text.isEmpty())
           {
            m_actionName = QString("ACTION_%1").arg(text).toUpper();
            m_actionName.remove("&");
            this->setObjectName(m_actionName);
           }
        }
}

CAction::CAction(const QString &text, const QString &iconName, const QString &shortCut, const QString &name, QObject *parent) : QAction(text, parent)
{
    m_actionName = name;
    m_iconName = iconName;
    m_text = text;

    if (CStyle::iconFromStyleExist(m_iconName))
        this->setIcon(CStyle::iconFromStyle(m_iconName));
    else
        this->setIcon(QIcon::fromTheme(m_iconName));

    if (!shortCut.isEmpty())
        this->setShortcut( QKeySequence(shortCut));
    if (!name.isEmpty())
       {
        m_actionName = name;
        this->setObjectName(m_actionName);
       }
    else
       {
        if (!text.isEmpty())
           {
            m_actionName = QString("ACTION_%1").arg(text).toUpper();
            m_actionName.remove("&");
            this->setObjectName(m_actionName);
           }
        }
}

QString CAction::getActionIcon()
{
    QString iconPath = QString("");

    if (!m_iconName.isEmpty())
        iconPath = QString("%1/%2.png").arg(ICONS_PATH).arg(m_iconName);
    return iconPath;
}
