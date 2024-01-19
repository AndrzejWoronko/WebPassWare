#ifndef CAUTOTYPEPLATFORMTEST_H
#define CAUTOTYPEPLATFORMTEST_H

#include "AutoTypePlatform.h"
#include "AutoTypeAction.h"

class CAutoTypePlatformTest : public QObject, public CAutoTypePlatform
{
  Q_OBJECT

public:
  CAutoTypePlatformTest(QObject *parent = NULL);

  QString keyToString(Qt::Key key);

  bool isAvailable() override;
  QStringList windowTitles() override;
  WId activeWindow() override;
  QString activeWindowTitle() override;
/*
  bool registerGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers) override;
  void unregisterGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers) override;
  int platformEventFilter(void* event) override;
  int initialTimeout() override;
 */

  bool raiseWindow(WId window) override;
  CAutoTypeExecutor* createExecutor() override;

#ifdef Q_OS_MACOS
    bool hideOwnWindow() override;
    bool raiseOwnWindow() override;
#endif

  void setActiveWindowTitle(const QString& title);

  QString actionChars();
  int actionCount();
  void clearActions();

  void addActionChar(CAutoTypeChar* action);
  void addActionKey(CAutoTypeKey* action);

Q_SIGNALS:
  void globalShortcutTriggered();

private:
  QString m_activeWindowTitle;
  QList<CAutoTypeAction*> m_actionList;
  QString m_actionChars;
};

class CAutoTypeExecturorTest : public CAutoTypeExecutor
{
public:
    explicit CAutoTypeExecturorTest(CAutoTypePlatformTest* platform);

    void execChar(CAutoTypeChar* action) override;
    void execKey(CAutoTypeKey* action) override;

private:
    CAutoTypePlatformTest* const m_platform;
};



#endif // CAUTOTYPEPLATFORMTEST_H
