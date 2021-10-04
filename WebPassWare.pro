include( includes.pri ) {
    error( WebPassWare.pro: Cannot find the includes.pri file! )
}

TARGET = webpassware
TEMPLATE = app

SOURCES += \
	 main.cpp \

HEADERS += \
	 Global.h \



FORMS += \


RESOURCES += \


INCLUDEPATH += src/

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

