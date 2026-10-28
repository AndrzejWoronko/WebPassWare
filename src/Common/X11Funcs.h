#ifndef X11FUNCS_H
#define X11FUNCS_H

#include <Qt>
#include <QChar>

#ifdef Q_OS_LINUX  //Linux

#include <X11/X.h>

namespace wpw {

KeySym qcharToNativeKeyCode(const QChar& ch);
KeySym qtToNativeKeyCode(Qt::Key key);

} // namespace wpw

#endif
#endif // X11FUNCS_H
