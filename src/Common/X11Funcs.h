#ifndef X11FUNCS_H
#define X11FUNCS_H

#include <Qt>
#include <QChar>

#ifdef Q_OS_LINUX  //Linux

#include <X11/X.h>

KeySym qcharToNativeKeyCode(const QChar& ch);
KeySym qtToNativeKeyCode(Qt::Key key);

#endif
#endif // X11FUNCS_H
