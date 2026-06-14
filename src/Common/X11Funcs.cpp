#include "X11Funcs.h"

#ifdef Q_OS_LINUX  //Linux

#include <X11/keysym.h>

KeySym qcharToNativeKeyCode(const QChar& ch)
{
    // Unicode code points can be converted to keysym using the following scheme,
    // see also the X11 function XConvertCase()
    if (ch.unicode() <= 0x1ff) {
        return ch.unicode();
    } else {
        return ch.unicode() + 0x01000000;
    }
}

KeySym qtToNativeKeyCode(Qt::Key key)
{
    switch (key) {
    case Qt::Key_Escape:
        return XK_Escape;
    case Qt::Key_Tab:
        return XK_Tab;
    case Qt::Key_Backtab:
        return XK_ISO_Left_Tab;
    case Qt::Key_Backspace:
        return XK_BackSpace;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        return XK_Return;
    case Qt::Key_Insert:
        return XK_Insert;
    case Qt::Key_Delete:
        return XK_Delete;
    case Qt::Key_Pause:
        return XK_Pause;
    case Qt::Key_Print:
        return XK_Print;
    case Qt::Key_Home:
        return XK_Home;
    case Qt::Key_End:
        return XK_End;
    case Qt::Key_Left:
        return XK_Left;
    case Qt::Key_Up:
        return XK_Up;
    case Qt::Key_Right:
        return XK_Right;
    case Qt::Key_Down:
        return XK_Down;
    case Qt::Key_PageUp:
        return XK_Page_Up;
    case Qt::Key_PageDown:
        return XK_Page_Down;
    case Qt::Key_CapsLock:
        return XK_Caps_Lock;
    case Qt::Key_NumLock:
        return XK_Num_Lock;
    case Qt::Key_ScrollLock:
        return XK_Scroll_Lock;
    case Qt::Key_F1:
        return XK_F1;
    case Qt::Key_F2:
        return XK_F2;
    case Qt::Key_F3:
        return XK_F3;
    case Qt::Key_F4:
        return XK_F4;
    case Qt::Key_F5:
        return XK_F5;
    case Qt::Key_F6:
        return XK_F6;
    case Qt::Key_F7:
        return XK_F7;
    case Qt::Key_F8:
        return XK_F8;
    case Qt::Key_F9:
        return XK_F9;
    case Qt::Key_F10:
        return XK_F10;
    case Qt::Key_F11:
        return XK_F11;
    case Qt::Key_F12:
        return XK_F12;
    case Qt::Key_F13:
        return XK_F13;
    case Qt::Key_F14:
        return XK_F14;
    case Qt::Key_F15:
        return XK_F15;
    case Qt::Key_F16:
        return XK_F16;
    case Qt::Key_Menu:
        return XK_Menu;
    case Qt::Key_Help:
        return XK_Help;
    case Qt::Key_Space:
        return XK_space;
    default:
        return NoSymbol;
    }
}

#endif
