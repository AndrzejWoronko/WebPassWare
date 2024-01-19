#ifndef CAUTOTYPEPLATFORMWINDOWS_H
#define CAUTOTYPEPLATFORMWINDOWS_H

#undef NOMINMAX
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Global.h"
#include "AutoTypeAction.h"
#include "AutoTypePlatform.h"

class CAutoTypePlatformWindows : public QObject, public CAutoTypePlatform
{
    Q_OBJECT

public:
    CAutoTypePlatformWindows(QObject *parent = NULL);
    bool isAvailable() override;
    QStringList windowTitles() override;
    WId activeWindow() override;
    QString activeWindowTitle() override;
    bool raiseWindow(WId window) override;
    CAutoTypeExecutor* createExecutor() override;

    void sendCharVirtual(const QChar& ch);
    void sendChar(const QChar& ch);
    void setKeyState(Qt::Key key, bool down);

private:
    static bool isExtendedKey(DWORD nativeKeyCode);
    static bool isAltTabWindow(HWND hwnd);
    static BOOL CALLBACK windowTitleEnumProc(_In_ HWND hwnd, _In_ LPARAM lParam);
    static QString windowTitle(HWND hwnd);
    WORD qtToNativeKeyCode(Qt::Key key);
};

class CAutoTypeExecutorWindows : public CAutoTypeExecutor
{
public:
    explicit CAutoTypeExecutorWindows(CAutoTypePlatformWindows* platform);

    void execChar(CAutoTypeChar* action) override;
    void execKey(CAutoTypeKey* action) override;

private:
    CAutoTypePlatformWindows* const m_platform;
};

#endif // CAUTOTYPEPLATFORMWINDOWS_H
