include( includes.pri ) {
    error( WebPassWare.pro: Cannot find the includes.pri file! )
}

TARGET = webpassware
TEMPLATE = app

SOURCES += \
         src/main.cpp \

HEADERS += \
         src/Common/Global.h \
         src/Common/Singleton.h \


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

