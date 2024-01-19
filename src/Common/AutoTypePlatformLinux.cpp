#include "AutoTypePlatformLinux.h"
#include "Tools.h"

#ifdef Q_OS_LINUX  //Linux
#include "gui/osutils/nixutils/X11Funcs.h"

#include <QX11Info>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

/* map of ASCII non-dead keys to equivalent dead keys that need to be repeated */
static const QPair<KeySym, KeySym> deadMap[] = {
    {XK_acute, XK_dead_acute},
    {XK_grave, XK_dead_grave},
    {XK_asciicircum, XK_dead_circumflex},
    {XK_asciitilde, XK_dead_tilde},
    {XK_asciitilde, XK_dead_perispomeni},
};

CAutoTypePlatformLinux::CAutoTypePlatformLinux(QObject *parent) : QObject(parent)
{
  // Qt handles XCB slightly differently so we open our own connection
  m_dpy = XOpenDisplay(XDisplayString(QX11Info::display()));
  m_rootWindow = QX11Info::appRootWindow();

  m_atomWmState = XInternAtom(m_dpy, "WM_STATE", True);
  m_atomWmName = XInternAtom(m_dpy, "WM_NAME", True);
  m_atomNetWmName = XInternAtom(m_dpy, "_NET_WM_NAME", True);
  m_atomString = XInternAtom(m_dpy, "STRING", True);
  m_atomUtf8String = XInternAtom(m_dpy, "UTF8_STRING", True);
  m_atomNetActiveWindow = XInternAtom(m_dpy, "_NET_ACTIVE_WINDOW", True);
  m_atomTransientFor = XInternAtom(m_dpy, "WM_TRANSIENT_FOR", True);
  m_atomWindow = XInternAtom(m_dpy, "WINDOW", True);

  m_classBlacklist << "desktop_window"
                   << "gnome-panel"; // Gnome
  m_classBlacklist << "kdesktop"
                   << "kicker"; // KDE 3
  m_classBlacklist << "Plasma"; // KDE 4
  m_classBlacklist << "plasmashell"; // KDE 5
  m_classBlacklist << "xfdesktop"
                   << "xfce4-panel"; // Xfce 4

  m_xkb = nullptr;

  m_loaded = true;
}

bool CAutoTypePlatformLinux::isAvailable()
{
  int ignore;

  if (!XQueryExtension(m_dpy, "XInputExtension", &ignore, &ignore, &ignore)) {
      return false;
  }

  if (!XQueryExtension(m_dpy, "XTEST", &ignore, &ignore, &ignore)) {
      return false;
  }

  return true;
}

void CAutoTypePlatformLinux::unload()
{
  m_keymap.clear();

  if (m_xkb) {
      XkbFreeKeyboard(m_xkb, XkbAllComponentsMask, True);
      m_xkb = nullptr;
  }

  XCloseDisplay(m_dpy);
  m_dpy = nullptr;

  m_loaded = false;
}

QStringList CAutoTypePlatformLinux::windowTitles()
{
    return windowTitlesRecursive(m_rootWindow);
}

WId CAutoTypePlatformLinux::activeWindow()
{
  Window window;
  int revert_to_return;
  XGetInputFocus(m_dpy, &window, &revert_to_return);

  int tree;
  do {
      if (isTopLevelWindow(window)) {
          break;
      }

      Window root;
      Window parent;
      Window* children = nullptr;
      unsigned int numChildren;
      tree = XQueryTree(m_dpy, window, &root, &parent, &children, &numChildren);
      window = parent;
      if (children) {
          XFree(children);
      }
  } while (tree && window);

  return window;
}

QString CAutoTypePlatformLinux::activeWindowTitle()
{
    return windowTitle(activeWindow(), true);
}

CAutoTypeExecutor* CAutoTypePlatformLinux::createExecutor()
{
    return new CAutoTypeExecturorLinux(this);
}


QString CAutoTypePlatformLinux::windowTitle(Window window, bool useBlacklist)
{
  QString title;

  Atom type;
  int format;
  unsigned long nitems;
  unsigned long after;
  unsigned char* data = nullptr;

  // the window manager spec says we should read _NET_WM_NAME first, then fall back to WM_NAME

  int retVal = XGetWindowProperty(
      m_dpy, window, m_atomNetWmName, 0, 1000, False, m_atomUtf8String, &type, &format, &nitems, &after, &data);

  if ((retVal == 0) && data) {
      title = QString::fromUtf8(reinterpret_cast<char*>(data));
  } else {
      XTextProperty textProp;
      retVal = XGetTextProperty(m_dpy, window, &textProp, m_atomWmName);
      if ((retVal != 0) && textProp.value) {
          char** textList = nullptr;
          int count;

          if (textProp.encoding == m_atomUtf8String) {
              title = QString::fromUtf8(reinterpret_cast<char*>(textProp.value));
          } else if ((XmbTextPropertyToTextList(m_dpy, &textProp, &textList, &count) == 0) && textList
                     && (count > 0)) {
              title = QString::fromLocal8Bit(textList[0]);
          } else if (textProp.encoding == m_atomString) {
              title = QString::fromLocal8Bit(reinterpret_cast<char*>(textProp.value));
          }

          if (textList) {
              XFreeStringList(textList);
          }
      }

      if (textProp.value) {
          XFree(textProp.value);
      }
  }

  if (data) {
      XFree(data);
  }

  if (useBlacklist && !title.isEmpty()) {
      if (window == m_rootWindow) {
          return QString();
      }

      QString className = windowClassName(window);
      if (m_classBlacklist.contains(className)) {
          return QString();
      }

      QList<Window> keepassxWindows = widgetsToX11Windows(QApplication::topLevelWidgets());
      if (keepassxWindows.contains(window)) {
          return QString();
      }
  }

  return title;
}

QString CAutoTypePlatformLinux::windowClassName(Window window)
{
  QString className;

  XClassHint wmClass;
  wmClass.res_name = nullptr;
  wmClass.res_class = nullptr;

  if (XGetClassHint(m_dpy, window, &wmClass) && wmClass.res_name) {
      className = QString::fromLocal8Bit(wmClass.res_name);
  }
  if (wmClass.res_name) {
      XFree(wmClass.res_name);
  }
  if (wmClass.res_class) {
      XFree(wmClass.res_class);
  }

  return className;
}

QList<Window> CAutoTypePlatformLinux::widgetsToX11Windows(const QWidgetList& widgetList)
{
    QList<Window> windows;

    for (const QWidget* widget : widgetList) {
        windows.append(widget->effectiveWinId());
    }

    return windows;
}

QStringList CAutoTypePlatformLinux::windowTitlesRecursive(Window window)
{
    QStringList titles;

    if (isTopLevelWindow(window)) {
        QString title = windowTitle(window, true);
        if (!title.isEmpty()) {
            titles.append(title);
        }
    }

    Window root;
    Window parent;
    Window* children = nullptr;
    unsigned int numChildren;
    if (XQueryTree(m_dpy, window, &root, &parent, &children, &numChildren) && children) {
        for (uint i = 0; i < numChildren; i++) {
            titles.append(windowTitlesRecursive(children[i]));
        }
    }
    if (children) {
        XFree(children);
    }

    return titles;
}

bool CAutoTypePlatformLinux::isTopLevelWindow(Window window)
{
  bool result = false;

  Atom type = None;
  int format;
  unsigned long nitems;
  unsigned long after;
  unsigned char* data = nullptr;

  // Check if the window has WM_STATE atom and it is not Withdrawn
  int retVal = XGetWindowProperty(
      m_dpy, window, m_atomWmState, 0, 2, False, m_atomWmState, &type, &format, &nitems, &after, &data);

  if (retVal == 0 && data) {
      if (type == m_atomWmState && format == 32 && nitems > 0) {
          result = (static_cast<quint32>(*data) != WithdrawnState);
      }
      XFree(data);
  } else {
      // See if this is a transient window without WM_STATE
      retVal = XGetWindowProperty(
          m_dpy, window, m_atomTransientFor, 0, 1, False, m_atomWindow, &type, &format, &nitems, &after, &data);
      if (retVal == 0 && data) {
          result = true;
          XFree(data);
      }
  }

  return result;
}

/*
 * Update the keyboard and modifier mapping.
 * We need the KeyboardMapping for AddKeysym.
 * Modifier mapping is required for clearing the modifiers.
 */
void CAutoTypePlatformLinux::updateKeymap()
{
    if (m_xkb) {
        XkbFreeKeyboard(m_xkb, XkbAllComponentsMask, True);
    }
    m_xkb = XkbGetMap(m_dpy, XkbAllClientInfoMask, XkbUseCoreKbd);

    /* workaround X11 bug https://gitlab.freedesktop.org/xorg/xserver/-/issues/1155 */
    XkbSetMap(m_dpy, XkbAllClientInfoMask, m_xkb);
    XSync(m_dpy, False);

    /* Build updated keymap */
    m_keymap.clear();
    m_remapKeycode = 0;

    for (int ckeycode = m_xkb->min_key_code; ckeycode < m_xkb->max_key_code; ckeycode++) {
        int groups = XkbKeyNumGroups(m_xkb, ckeycode);

        /* track highest remappable keycode, don't add to keymap */
        if (groups == 0) {
            m_remapKeycode = ckeycode;
            continue;
        }

        for (int cgroup = 0; cgroup < groups; cgroup++) {
            XkbKeyTypePtr type = XkbKeyKeyType(m_xkb, ckeycode, cgroup);

            for (int clevel = 0; clevel < type->num_levels; clevel++) {
                KeySym sym = XkbKeycodeToKeysym(m_dpy, ckeycode, cgroup, clevel);

                int mask = 0;
                for (int nmap = 0; nmap < type->map_count; nmap++) {
                    XkbKTMapEntryRec map = type->map[nmap];
                    if (map.active && map.level == clevel) {
                        mask = map.mods.mask;
                        break;
                    }
                }

                /* explicitly disallow requiring lock modifiers (Caps Lock and Num Lock) */
                if (mask & (LockMask | Mod2Mask)) {
                    continue;
                }

                m_keymap.append(CAutoTypePlatformLinux::KeyDesc{sym, ckeycode, cgroup, mask});
            }
        }
    }

    /* determine the keycode to use for modifiers */
    XModifierKeymap* modifiers = XGetModifierMapping(m_dpy);
    for (int mod_index = ShiftMapIndex; mod_index <= Mod5MapIndex; mod_index++) {
        m_modifier_keycode[mod_index] = 0;
        for (int mod_key = 0; mod_key < modifiers->max_keypermod; mod_key++) {
            int keycode = modifiers->modifiermap[mod_index * modifiers->max_keypermod + mod_key];
            if (keycode) {
                m_modifier_keycode[mod_index] = keycode;
                break;
            }
        }
    }
    XFreeModifiermap(modifiers);
}

// --------------------------------------------------------------------------
// The following code is taken from xvkbd 3.0 and has been slightly modified.
// --------------------------------------------------------------------------

/*
 * Send event to the focused window.
 * If input focus is specified explicitly, select the window
 * before send event to the window.
 */
void CAutoTypePlatformLinux::SendKeyEvent(unsigned keycode, bool press)
{
    XSync(m_dpy, False);
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(MyErrorHandler);

    XTestFakeKeyEvent(m_dpy, keycode, press, 0);
    XFlush(m_dpy);
    XSync(m_dpy, False);

    XSetErrorHandler(oldHandler);
}

/*
 * Send a modifier press/release event for all modifiers
 * which are set in the mask variable.
 */
void CAutoTypePlatformLinux::SendModifiers(unsigned int mask, bool press)
{
    int mod_index;
    for (mod_index = ShiftMapIndex; mod_index <= Mod5MapIndex; mod_index++) {
        if (mask & (1 << mod_index)) {
            SendKeyEvent(m_modifier_keycode[mod_index], press);
        }
    }
}

/*
 * Determines the keycode and modifier mask for the given
 * keysym.
 */
bool CAutoTypePlatformLinux::GetKeycode(KeySym keysym, int* keycode, int* group, unsigned int* mask, bool* repeat)
{
    const KeyDesc* desc = nullptr;
    bool isDead = false;

    for (const auto& key : m_keymap) {
        if (key.sym == keysym) {
            // pick this description if we don't have any for this sym or this matches the current group
            if (desc == nullptr || key.group == *group) {
                desc = &key;
            }
        }
    }

    // try to find the best dead key mapping if we're unlucky to have one
    if (!desc) {
        for (const auto& map : deadMap) {
            if (map.first == keysym) {
                for (const auto& key : m_keymap) {
                    if (key.sym == map.second) {
                        // same as above, we try to match the group so no breaking out
                        if (desc == nullptr || key.group == *group) {
                            desc = &key;
                            isDead = true;
                        }
                    }
                }
            }
        }
    }

    if (desc) {
        *keycode = desc->code;
        *group = desc->group;
        *mask = desc->mask;
        *repeat = isDead;
        return true;
    }

    /* if we can't find an existing key for this keysym, try remapping */
    if (RemapKeycode(keysym)) {
        *keycode = m_remapKeycode;
        *group = 0;
        *mask = 0;
        *repeat = false;
        return true;
    }

    return false;
}

/*
 * Get remapped keycode for any keysym.
 */
bool CAutoTypePlatformLinux::RemapKeycode(KeySym keysym)
{
    if (!m_remapKeycode) {
        return false;
    }

    if (keysym != NoSymbol) {
        int type = XkbOneLevelIndex;
        if (XkbChangeTypesOfKey(m_xkb, m_remapKeycode, 1, XkbGroup1Mask, &type, NULL) != Success) {
            return false;
        }

        XkbKeySymEntry(m_xkb, m_remapKeycode, 0, 0) = keysym;
    } else {
        XkbChangeTypesOfKey(m_xkb, m_remapKeycode, 0, XkbGroup1Mask, NULL, NULL);
    }

    XkbSetMap(m_dpy, XkbAllClientInfoMask, m_xkb);
    XFlush(m_dpy);
    XSync(m_dpy, False);
    return true;
}

/*
 * Send sequence of KeyPressed/KeyReleased events to the focused
 * window to simulate keyboard.  If modifiers (shift, control, etc)
 * are set ON, many events will be sent.
 */
void CAutoTypePlatformLinux::sendKey(KeySym keysym, unsigned int modifiers)
{
    if (keysym == NoSymbol)
    {
        DEBUG_WITH_LINE << tr("Trying to send invalid keysym.");
        return;
    }

    int keycode;
    int group;
    int group_active;
    unsigned int wanted_mask;
    bool repeat;

    /* pull current active layout group */
    XkbStateRec state;
    XkbGetState(m_dpy, XkbUseCoreKbd, &state);
    group_active = state.group;

    /* tell GetKeycode we would prefer a key from active group */
    group = group_active;

    Window root, child;
    int root_x, root_y, x, y;
    unsigned int original_mask;

    XSync(m_dpy, False);
    XQueryPointer(m_dpy, m_rootWindow, &root, &child, &root_x, &root_y, &x, &y, &original_mask);

    /* fail permanently if Caps Lock is on */
    if (original_mask & LockMask)
    {
        DEBUG_WITH_LINE << tr("Sequence aborted: Caps Lock is on");
        return;
    }

    /* retry if keysym affecting modifier is held except Num Lock (Mod2Mask) */
    if (original_mask & (ShiftMask | ControlMask | Mod1Mask | Mod3Mask | Mod4Mask | Mod5Mask))
    {
        DEBUG_WITH_LINE << tr("Sequence aborted: Modifier keys held by user");
        return;
    }

    /* determine keycode, group and mask for the given keysym */
    if (!GetKeycode(keysym, &keycode, &group, &wanted_mask, &repeat))
    {
        DEBUG_WITH_LINE << tr("Unable to get valid keycode for key: ") + QString(XKeysymToString(keysym));
        return;
    }

    wanted_mask |= modifiers;

    /* modifiers that need to be held but aren't */
    unsigned int press_mask = wanted_mask & ~original_mask;

    /* change layout group if necessary */
    if (group_active != group) {
        XkbLockGroup(m_dpy, XkbUseCoreKbd, group);
        XFlush(m_dpy);
    }

    /* hold modifiers */
    SendModifiers(press_mask, true);

    /* press and release key, with repeat if necessary */
    for (int i = 0; i < (repeat ? 2 : 1); i++) {
        SendKeyEvent(keycode, true);
        SendKeyEvent(keycode, false);
    }

    /* release modifiers */
    SendModifiers(press_mask, false);

    /* reset layout group if necessary */
    if (group_active != group) {
        XkbLockGroup(m_dpy, XkbUseCoreKbd, group_active);
        XFlush(m_dpy);
    }

    /* reset remap to prevent leaking remap keysyms longer than necessary */
    if (keycode == m_remapKeycode) {
        RemapKeycode(NoSymbol);
    }
}

int CAutoTypePlatformLinux::MyErrorHandler(Display* my_dpy, XErrorEvent* event)
{
    char msg[200];

    if (event->error_code == BadWindow) {
        return 0;
    }
    XGetErrorText(my_dpy, event->error_code, msg, sizeof(msg) - 1);
    qWarning("X error trapped: %s, request-code=%d\n", msg, event->request_code);
    return 0;
}

bool CAutoTypePlatformLinux::raiseWindow(WId window)
{
    if (m_atomNetActiveWindow == None) {
        return false;
    }

    XRaiseWindow(m_dpy, window);

    XEvent event;
    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = True;
    event.xclient.window = window;
    event.xclient.message_type = m_atomNetActiveWindow;
    event.xclient.format = 32;
    event.xclient.data.l[0] = 1; // FromApplication
    event.xclient.data.l[1] = QX11Info::appUserTime();
    QWidget* activeWindow = QApplication::activeWindow();
    if (activeWindow) {
        event.xclient.data.l[2] = activeWindow->internalWinId();
    } else {
        event.xclient.data.l[2] = 0;
    }
    event.xclient.data.l[3] = 0;
    event.xclient.data.l[4] = 0;
    XSendEvent(m_dpy, m_rootWindow, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
    XFlush(m_dpy);

    return true;
}

int CAutoTypePlatformLinux::initialTimeout()
{
    return 500;
}

CAutoTypeExecturorLinux::CAutoTypeExecturorLinux(CAutoTypePlatformLinux* platform)
    : m_platform(platform)
{
}

void CAutoTypeExecturorLinux::execChar(CAutoTypeChar* action)
{
  m_platform->sendKey(qcharToNativeKeyCode(action->m_character));
}

void CAutoTypeExecturorLinux::execKey(CAutoTypeKey* action)
{
  m_platform->sendKey(qtToNativeKeyCode(action->m_key));
}

#endif
