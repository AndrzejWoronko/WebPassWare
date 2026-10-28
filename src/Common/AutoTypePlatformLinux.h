#ifndef CAUTOTYPEPLATFORMLINUX_H
#define CAUTOTYPEPLATFORMLINUX_H

#include <QApplication>
#include <QSet>
#include <QtPlugin>
#include <QWidget>
#include "Global.h"

#ifdef Q_OS_LINUX  //Linux

#include "AutoTypePlatform.h"
#include "AutoTypeAction.h"

// The X11 types used below, declared exactly as Xlib declares them, so that
// this header does not bring Xlib and its macros (Bool, None, Status) to every
// file that includes it, the shared moc file among them.
typedef struct _XDisplay Display;
typedef unsigned long Window;
typedef unsigned long Atom;
typedef unsigned long KeySym;
typedef unsigned char KeyCode;
struct _XkbDesc;
typedef struct _XkbDesc *XkbDescPtr;

// Mod5MapIndex + 1 (checked in the .cpp).
#define N_MOD_INDICES 8

namespace wpw {

class CAutoTypePlatformLinux : public QObject, public CAutoTypePlatform
{
    Q_OBJECT

public:
    CAutoTypePlatformLinux(QObject *parent = nullptr);
    // Closes the X11 connection unless unload() already did.
    ~CAutoTypePlatformLinux() override;
    bool isAvailable() override;
    void unload() override;
    QStringList windowTitles() override;
    WId activeWindow() override;
    QString activeWindowTitle() override;
    bool raiseWindow(WId window) override;
    CAutoTypeExecutor* createExecutor() override;

    void updateKeymap();


    //KeySym charToKeySym(const QChar& ch);
    //KeySym keyToKeySym(Qt::Key key);

    void sendKey(KeySym keysym, unsigned int modifiers = 0);

Q_SIGNALS:
    void globalShortcutTriggered();

private:
    QString windowTitle(Window window, bool useBlacklist);
    QStringList windowTitlesRecursive(Window window);
    QString windowClassName(Window window);
    QList<Window> widgetsToX11Windows(const QWidgetList& widgetList);
    bool isTopLevelWindow(Window window);


    XkbDescPtr getKeyboard();
    bool RemapKeycode(KeySym keysym);
    void SendKeyEvent(unsigned keycode, bool press);
    void SendModifiers(unsigned int mask, bool press);
    bool GetKeycode(KeySym keysym, int* keycode, int* group, unsigned int* mask, bool* repeat);

    Display* m_dpy;
    Window m_rootWindow;
    Atom m_atomWmState;
    Atom m_atomWmName;
    Atom m_atomNetWmName;
    Atom m_atomString;
    Atom m_atomUtf8String;
    Atom m_atomNetActiveWindow;
    Atom m_atomTransientFor;
    Atom m_atomWindow;
    QSet<QString> m_classBlacklist;

    typedef struct
    {
        KeySym sym;
        int code;
        int group;
        int mask;
    } KeyDesc;

    XkbDescPtr m_xkb;
    QList<KeyDesc> m_keymap;
    KeyCode m_modifier_keycode[N_MOD_INDICES];
    KeyCode m_remapKeycode;
    bool m_loaded;
};

} // namespace wpw

#endif
#endif // CAUTOTYPEPLATFORMLINUX_H
