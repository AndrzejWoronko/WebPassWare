#ifndef MACUTILS_H
#define MACUTILS_H

#include <Qt>

#ifdef Q_OS_MACOS //Macos

#include <sys/types.h>

namespace wpw {

bool macUtilsRaiseProcess(pid_t pid);
bool macUtilsHideOwnApp();
bool macUtilsRaiseOwnApp();

} // namespace wpw

#endif
#endif // MACUTILS_H
