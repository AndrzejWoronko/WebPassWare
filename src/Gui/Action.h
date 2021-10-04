#ifndef ACTION_H
#define ACTION_H

#include "Global.h"
#include <QAction>
#include <QIcon>


//action slot name :
// on_ACTION_NAME_triggered()

class CAction : public QAction
{
      Q_OBJECT
      ADD_FIELD(QString, m_actionName, getActionName, setActionName)
      ADD_FIELD(QString, m_text, getText, setText)
      ADD_FIELD(QString, m_iconName, getIconName, setIconName)

    public:
      CAction(QObject *parent = NULL);
      CAction(const QString &text, const QString &shortCut = QString(), const QString &name = QString(), QObject *parent = NULL);
      CAction(const QString &text, const QString &iconName = QString(), const QString &shortCut = QString(),
              const QString &name = QString(), QObject *parent = NULL);
      CAction(const QString &text, const QString &iconName = QString(), const QString &iconText = QString(),
              const QString &shortCut = QString(), const QString &name = QString(), QObject *parent = NULL);

      QString getActionIcon();
    private:


};

#endif
