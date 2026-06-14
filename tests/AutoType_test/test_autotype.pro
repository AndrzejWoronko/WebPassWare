#-------------------------------------------------
#
# AutoType test project
#
#-------------------------------------------------
QT += core gui testlib widgets
CONFIG += qt console
CONFIG -= app_bundle

TARGET = test_autotype

TEMPLATE = app

INCLUDEPATH += ../../src/ ../../src/Common/

DEPENDPATH  += $${INCLUDEPATH}

HEADERS += ../../src/Common/AutoType.h \
    ../../src/Common/AutoTypeAction.h \
    ../../src/Common/AutoTypePlatform.h \
    ../../src/Common/AutoTypePlatformLinux.h \
    ../../src/Common/AutoTypePlatformTest.h \
    ../../src/Common/X11Funcs.h

SOURCES += ../../src/Common/AutoType.cpp \
    ../../src/Common/AutoTypeAction.cpp \
    ../../src/Common/AutoTypePlatform.cpp \
    ../../src/Common/AutoTypePlatformLinux.cpp \
    ../../src/Common/AutoTypePlatformTest.cpp \
    ../../src/Common/Tools.cpp \
    ../../src/Common/X11Funcs.cpp \
    test_autotype.cpp

unix:!macx {
    QT += x11extras
    LIBS += -lX11 -lXtst
}

DEFINES += SRCDIR=\\\"$$PWD/\\\"

! include( ../test_clean.pri ) {
    error( test_autotype.pro: Cannot find the test_clean.pri file! )
}
