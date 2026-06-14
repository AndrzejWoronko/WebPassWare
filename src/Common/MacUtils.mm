#include "MacUtils.h"

#ifdef Q_OS_MACOS //Macos

#import <AppKit/AppKit.h>

//
// Activate (bring to front) the application owning the given process id
//
bool macUtilsRaiseProcess(pid_t pid)
{
    NSRunningApplication* app = [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
    if (!app) {
        return false;
    }

    return [app activateWithOptions:NSApplicationActivateIgnoringOtherApps];
}

//
// Hide our own application (used to give focus back to the target window)
//
bool macUtilsHideOwnApp()
{
    [NSApp hide:nil];
    return true;
}

//
// Unhide and activate our own application
//
bool macUtilsRaiseOwnApp()
{
    [NSApp unhide:nil];
    return [[NSRunningApplication currentApplication] activateWithOptions:NSApplicationActivateIgnoringOtherApps];
}

#endif
