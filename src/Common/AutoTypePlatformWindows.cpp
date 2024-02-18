#include "AutoTypePlatformWindows.h"
#include "Tools.h"

#ifdef Q_OS_WINDOWS //Windows

#include <VersionHelpers.h>

#define HOTKEY_ID 1
#define MAX_WINDOW_TITLE_LENGTH 1024
#define MOD_NOREPEAT 0x4000 // Missing in MinGW



CAutoTypePlatformWindows::CAutoTypePlatformWindows(QObject *parent) : QObject(parent)
{

}

//
// Test if os version is Windows 7 or later
//
bool CAutoTypePlatformWindows::isAvailable()
{
    return IsWindows7OrGreater();
}

//
// Get list of all visible window titles
//
QStringList CAutoTypePlatformWindows::windowTitles()
{
    QStringList list;

    ::EnumWindows(CAutoTypePlatformWindows::windowTitleEnumProc, reinterpret_cast<LPARAM>(&list));

    return list;
}

//
// Get foreground window hwnd
//
WId CAutoTypePlatformWindows::activeWindow()
{
    return reinterpret_cast<WId>(::GetForegroundWindow());
}

//
// Get foreground window title
//
QString CAutoTypePlatformWindows::activeWindowTitle()
{
    return windowTitle(::GetForegroundWindow());
}

CAutoTypeExecutor* CAutoTypePlatformWindows::createExecutor()
{
    return new CAutoTypeExecutorWindows(this);
}

//
// Set foreground window
//
bool CAutoTypePlatformWindows::raiseWindow(WId window)
{
    HWND hwnd = reinterpret_cast<HWND>(window);

    return ::BringWindowToTop(hwnd) && ::SetForegroundWindow(hwnd);
}

//
// Send unicode character to foreground window
//
void CAutoTypePlatformWindows::sendChar(const QChar& ch)
{
    INPUT in[2];
    in[0].type = INPUT_KEYBOARD;
    in[0].ki.wVk = 0;
    in[0].ki.wScan = ch.unicode();
    in[0].ki.dwFlags = KEYEVENTF_UNICODE;
    in[0].ki.time = 0;
    in[0].ki.dwExtraInfo = ::GetMessageExtraInfo();

    in[1] = in[0];
    in[1].ki.dwFlags |= KEYEVENTF_KEYUP;

    ::SendInput(2, &in[0], sizeof(INPUT));
}

void CAutoTypePlatformWindows::sendCharVirtual(const QChar& ch)
{
    auto vKey = VkKeyScanExW(ch.unicode(), GetKeyboardLayout(0));
    if (vKey == -1) {
        // VKey not found, send as Unicode character
        sendChar(ch);
        return;
    }

    if (HIBYTE(vKey) & 0x6) {
        setKeyState(Qt::Key_AltGr, true);
    } else {
        if (HIBYTE(vKey) & 0x1) {
            setKeyState(Qt::Key_Shift, true);
        }
        if (HIBYTE(vKey) & 0x2) {
            setKeyState(Qt::Key_Control, true);
        }
        if (HIBYTE(vKey) & 0x4) {
            setKeyState(Qt::Key_Alt, true);
        }
    }

    INPUT in[2];
    in[0].type = INPUT_KEYBOARD;
    in[0].ki.wVk = 0;
    in[0].ki.wScan = MapVirtualKey(LOBYTE(vKey), MAPVK_VK_TO_VSC);
    in[0].ki.dwFlags = KEYEVENTF_SCANCODE;
    in[0].ki.time = 0;
    in[0].ki.dwExtraInfo = ::GetMessageExtraInfo();

    in[1] = in[0];
    in[1].ki.dwFlags |= KEYEVENTF_KEYUP;

    ::SendInput(2, &in[0], sizeof(INPUT));

    if (HIBYTE(vKey) & 0x6) {
        setKeyState(Qt::Key_AltGr, false);
    } else {
        if (HIBYTE(vKey) & 0x1) {
            setKeyState(Qt::Key_Shift, false);
        }
        if (HIBYTE(vKey) & 0x2) {
            setKeyState(Qt::Key_Control, false);
        }
        if (HIBYTE(vKey) & 0x4) {
            setKeyState(Qt::Key_Alt, false);
        }
    }
}

//
// Send virtual key code to foreground window
//
void CAutoTypePlatformWindows::setKeyState(Qt::Key key, bool down)
{
    WORD nativeKeyCode = this->qtToNativeKeyCode(key);
    DWORD nativeFlags = KEYEVENTF_SCANCODE;
    if (isExtendedKey(nativeKeyCode)) {
        nativeFlags |= KEYEVENTF_EXTENDEDKEY;
    }
    if (!down) {
        nativeFlags |= KEYEVENTF_KEYUP;
    }

    INPUT in;
    in.type = INPUT_KEYBOARD;
    in.ki.wVk = 0;
    in.ki.wScan = MapVirtualKey(LOBYTE(nativeKeyCode), MAPVK_VK_TO_VSC);
    in.ki.dwFlags = nativeFlags;
    in.ki.time = 0;
    in.ki.dwExtraInfo = ::GetMessageExtraInfo();

    ::SendInput(1, &in, sizeof(INPUT));
}

//
// The extended-key flag indicates whether the keystroke message originated
// from one of the additional keys on the enhanced keyboard
// see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646267%28v=vs.85%29.aspx#EXTENDED_KEY_FLAG
//
bool CAutoTypePlatformWindows::isExtendedKey(DWORD nativeKeyCode)
{
    switch (nativeKeyCode) {
    case VK_RMENU:
    case VK_RCONTROL:
    case VK_INSERT:
    case VK_DELETE:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_LEFT:
    case VK_UP:
    case VK_RIGHT:
    case VK_DOWN:
    case VK_NUMLOCK:
    case VK_CANCEL:
    case VK_SNAPSHOT:
    case VK_DIVIDE:
    case VK_LWIN:
    case VK_RWIN:
    case VK_APPS:
        return true;
    default:
        return false;
    }
}
// clang-format on

//
// Test if window is in Alt+Tab list
// see: https://blogs.msdn.microsoft.com/oldnewthing/20071008-00/?p=24863
//
bool CAutoTypePlatformWindows::isAltTabWindow(HWND hwnd)
{
    if (!::IsWindowVisible(hwnd)) {
        return false;
    }

    // Start at the root owner
    HWND hwndWalk = ::GetAncestor(hwnd, GA_ROOTOWNER);
    HWND hwndTry;

    // See if we are the last active visible popup
    while ((hwndTry = ::GetLastActivePopup(hwndWalk)) != hwndWalk) {
        if (::IsWindowVisible(hwndTry)) {
            break;
        }
        hwndWalk = hwndTry;
    }

    return hwndWalk == hwnd;
}

//
// Window title enum proc
//
BOOL CALLBACK CAutoTypePlatformWindows::windowTitleEnumProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
    if (!isAltTabWindow(hwnd)) {
        // Skip window
        return TRUE;
    }

    QStringList* list = reinterpret_cast<QStringList*>(lParam);
    QString title = windowTitle(hwnd);

    if (!title.isEmpty()) {
        list->append(title);
    }

    return TRUE;
}

//
// Get window title
//
QString CAutoTypePlatformWindows::windowTitle(HWND hwnd)
{
    wchar_t title[MAX_WINDOW_TITLE_LENGTH];
    int count = ::GetWindowTextW(hwnd, title, MAX_WINDOW_TITLE_LENGTH);

    return QString::fromUtf16(reinterpret_cast<const ushort*>(title), count);
}

// clang-format off
//
// Translate qt key code to windows virtual key code
// see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
//
WORD CAutoTypePlatformWindows::qtToNativeKeyCode(Qt::Key key)
{
    switch (key) {
    case Qt::Key_Backspace:
        return VK_BACK;     // 0x08
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        return VK_TAB;      // 0x09
    case Qt::Key_Clear:
        return VK_CLEAR;    // 0x0C
    case Qt::Key_Enter:
    case Qt::Key_Return:
        return VK_RETURN;   // 0x0D
    case Qt::Key_Shift:
        return VK_SHIFT;    // 0x10
    case Qt::Key_Control:
        return VK_CONTROL;  // 0x11
    case Qt::Key_Pause:
        return VK_PAUSE;    // 0x13
    case Qt::Key_CapsLock:
        return VK_CAPITAL;  // 0x14
    case Qt::Key_Escape:
        return VK_ESCAPE;   // 0x1B
    case Qt::Key_Space:
        return VK_SPACE;    // 0x20
    case Qt::Key_PageUp:
        return VK_PRIOR;    // 0x21
    case Qt::Key_PageDown:
        return VK_NEXT;     // 0x22
    case Qt::Key_End:
        return VK_END;      // 0x23
    case Qt::Key_Home:
        return VK_HOME;     // 0x24
    case Qt::Key_Left:
        return VK_LEFT;     // 0x25
    case Qt::Key_Up:
        return VK_UP;       // 0x26
    case Qt::Key_Right:
        return VK_RIGHT;    // 0x27
    case Qt::Key_Down:
        return VK_DOWN;     // 0x28
    case Qt::Key_Print:
        return VK_SNAPSHOT; // 0x2C
    case Qt::Key_Insert:
        return VK_INSERT;   // 0x2D
    case Qt::Key_Delete:
        return VK_DELETE;   // 0x2E
    case Qt::Key_Help:
        return VK_HELP;     // 0x2F
    case Qt::Key_Meta:
        return VK_LWIN;     // 0x5B
    case Qt::Key_AltGr:
        return VK_RMENU;    // 0xA5

    case Qt::Key_F1:
        return VK_F1;       // 0x70
    case Qt::Key_F2:
        return VK_F2;       // 0x71
    case Qt::Key_F3:
        return VK_F3;       // 0x72
    case Qt::Key_F4:
        return VK_F4;       // 0x73
    case Qt::Key_F5:
        return VK_F5;       // 0x74
    case Qt::Key_F6:
        return VK_F6;       // 0x75
    case Qt::Key_F7:
        return VK_F7;       // 0x76
    case Qt::Key_F8:
        return VK_F8;       // 0x77
    case Qt::Key_F9:
        return VK_F9;       // 0x78
    case Qt::Key_F10:
        return VK_F10;      // 0x79
    case Qt::Key_F11:
        return VK_F11;      // 0x7A
    case Qt::Key_F12:
        return VK_F12;      // 0x7B
    case Qt::Key_F13:
        return VK_F13;      // 0x7C
    case Qt::Key_F14:
        return VK_F14;      // 0x7D
    case Qt::Key_F15:
        return VK_F15;      // 0x7E
    case Qt::Key_F16:
        return VK_F16;      // 0x7F
    case Qt::Key_F17:
        return VK_F17;      // 0x80
    case Qt::Key_F18:
        return VK_F18;      // 0x81
    case Qt::Key_F19:
        return VK_F19;      // 0x82
    case Qt::Key_F20:
        return VK_F20;      // 0x83
    case Qt::Key_F21:
        return VK_F21;      // 0x84
    case Qt::Key_F22:
        return VK_F22;      // 0x85
    case Qt::Key_F23:
        return VK_F23;      // 0x86
    case Qt::Key_F24:
        return VK_F24;      // 0x87

    case Qt::Key_NumLock:
        return VK_NUMLOCK;  // 0x90
    case Qt::Key_ScrollLock:
        return VK_SCROLL;   // 0x91

    default:
        return LOBYTE(::VkKeyScanExW(key, ::GetKeyboardLayout(0)));
    }
}
// clang-format on

//
// ------------------------------ CAutoTypeExecutorWindows ------------------------------
//

CAutoTypeExecutorWindows::CAutoTypeExecutorWindows(CAutoTypePlatformWindows* platform)
    : m_platform(platform)
{
}

void CAutoTypeExecutorWindows::execChar(CAutoTypeChar* action)
{
    m_platform->sendChar(action->m_character);
}

void CAutoTypeExecutorWindows::execKey(CAutoTypeKey* action)
{
    m_platform->setKeyState(action->m_key, true);
    m_platform->setKeyState(action->m_key, false);
}
#endif
