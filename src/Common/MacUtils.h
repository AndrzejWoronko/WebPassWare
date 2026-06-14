#ifndef MACUTILS_H
#define MACUTILS_H

#include <Qt>

#ifdef Q_OS_MACOS //Macos

#include <sys/types.h>

bool macUtilsRaiseProcess(pid_t pid);
bool macUtilsHideOwnApp();
bool macUtilsRaiseOwnApp();

#endif
#endif // MACUTILS_H
