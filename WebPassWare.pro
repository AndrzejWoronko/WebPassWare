! include( includes.pri ) {
    error( WebPassWare.pro: Cannot find the includes.pri file! )
}

TARGET = webpassware
TEMPLATE = app

SOURCES += \
    src/Application.cpp \
    src/ApplicationSettings.cpp \
    src/Common/AbstractDialogStateManager.cpp \
    src/Common/AbstractSplitterStateManager.cpp \
    src/Common/AbstractTabMainWindowStateManager.cpp \
    src/Common/AbstractTableStateManager.cpp \
    src/Common/DialogState.cpp \
    src/Common/Logger.cpp \
    src/Common/SplitterState.cpp \
    src/Common/TabMainWindowState.cpp \
    src/Common/TableState.cpp \
    src/Common/Tools.cpp \
    src/Gui/AbstractMainWindow.cpp \
    src/Gui/Action.cpp \
    src/Gui/Layouts.cpp \
    src/Gui/MessageBox.cpp \
    src/Gui/Splitter.cpp \
    src/Gui/Style.cpp \
    src/WebPassWareMainWindow.cpp \
    src/main.cpp \

HEADERS += \
    src/Application.h \
    src/ApplicationSettings.h \
    src/Common/AbstractDialogStateManager.h \
    src/Common/AbstractSplitterStateManager.h \
    src/Common/AbstractTabMainWindowStateManager.h \
    src/Common/AbstractTableStateManager.h \
    src/Common/DialogState.h \
    src/Common/Global.h \
    src/Common/Logger.h \
    src/Common/Singleton.h \
    src/Common/SplitterState.h \
    src/Common/TabMainWindowState.h \
    src/Common/TableState.h \
    src/Common/Tools.h \
    src/Gui/AbstractMainWindow.h \
    src/Gui/Action.h \
    src/Gui/Layouts.h \
    src/Gui/MessageBox.h \
    src/Gui/Splitter.h \
    src/Gui/Style.h \
    src/WebPassWareMainWindow.h


FORMS += \


RESOURCES += \


INCLUDEPATH += src/ src/Common/ src/Gui/

DEPENDPATH  += $${INCLUDEPATH}

DEST_DIR = $${DEST_BINS}


macx {
    CONFIG  += app_bundle
}

win32 {
    EXTRA_DIR ~= s,/,\\,g
    DEST_DIR ~= s,/,\\,g

    DESTDIR = $$DEST_DIR
    RC_FILE += mainicon.rc
}
unix {
    DESTDIR = $$DEST_DIR
}

QMAKE_CXXFLAGS += -Wall -Wextra -std=gnu++11

QMAKE_LFLAGS_RPATH += \

