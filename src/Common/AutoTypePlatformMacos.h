#ifndef CAUTOTYPEPLATFORMMACOS_H
#define CAUTOTYPEPLATFORMMACOS_H

#include "Global.h"
#include <memory>

#ifdef Q_OS_MACOS //Macos

#include <Carbon/Carbon.h>

#include "AutoTypeAction.h"
#include "AutoTypePlatform.h"

class CAutoTypePlatformMacos : public QObject, public CAutoTypePlatform
{
    Q_OBJECT

public:
    CAutoTypePlatformMacos(QObject *parent = NULL);

    bool isAvailable() override;
    QStringList windowTitles() override;
    WId activeWindow() override;
    QString activeWindowTitle() override;
    bool raiseWindow(WId pid) override;
    CAutoTypeExecutor* createExecutor() override;

    bool hideOwnWindow() override;
    bool raiseOwnWindow() override;

    void sendChar(const QChar& ch, bool isKeyDown);
    void sendKey(Qt::Key key, bool isKeyDown, Qt::KeyboardModifiers modifiers);

private:
    static int windowLayer(CFDictionaryRef window);
    static QString windowStringProperty(CFDictionaryRef window, CFStringRef propertyRef);

    uint16 qtToNativeKeyCode(Qt::Key key);
    CGEventFlags qtToNativeModifiers(Qt::KeyboardModifiers modifiers, bool native);
};


class CAutoTypeExecutorMacos : public CAutoTypeExecutor
{

public:
    explicit CAutoTypeExecutorMacos(CAutoTypePlatformMacos* platform);

    void execChar(CAutoTypeChar* action) override;
    void execKey(CAutoTypeKey* action) override;

private:
    CAutoTypePlatformMacos* const m_platform;
};
#endif
#endif // CAUTOTYPEPLATFORMMACOS_H
