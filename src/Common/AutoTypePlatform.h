#ifndef CAUTOTYPEPLATFORM_H
#define CAUTOTYPEPLATFORM_H

#include <QWidget>

#include "AutoTypeAction.h"

class CAutoTypePlatform
{
public:
    CAutoTypePlatform();
    virtual ~CAutoTypePlatform() {}
    virtual bool isAvailable() = 0;
    virtual QStringList windowTitles() = 0;
    virtual WId activeWindow() = 0;
    virtual QString activeWindowTitle() = 0;
/*
    virtual bool registerGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers) = 0;
    virtual void unregisterGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers) = 0;
    virtual int platformEventFilter(void* event) = 0;    
    virtual int initialTimeout() = 0;
*/
    virtual bool raiseWindow(WId window) = 0;
    virtual void unload() {}

    virtual CAutoTypeExecutor* createExecutor() = 0;

#if defined(Q_OS_MACOS)
    virtual bool hideOwnWindow() = 0;
    virtual bool raiseOwnWindow() = 0;
#endif

    // implementations should also provide a globalShortcutTriggered() signal
};

#endif // CAUTOTYPEPLATFORM_H
