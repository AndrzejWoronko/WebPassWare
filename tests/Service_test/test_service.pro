QT += core gui testlib network sql xml script svg widgets
CONFIG += qt mmx sse stl exceptions warn_on

TARGET = test_service
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src/ ../../src/Common/ ../../src/Database/ \
        ../../src/Models/

DEPENDPATH  += $${INCLUDEPATH}

HEADERS += \
    ../../src/Database/SqlModel.h \
    ../../src/Models/PassEntry.h \
    ../../src/Models/PassGroup.h

SOURCES += \
    ../../src/Common/Logger.cpp \
    ../../src/Common/Tools.cpp \
    ../../src/Database/Database.cpp \
    ../../src/Database/Exception.cpp \
    ../../src/Database/ExceptionSql.cpp \
    ../../src/Database/OrmObject.cpp \
    ../../src/Database/Service.cpp \
    ../../src/Database/SqlField.cpp \
    ../../src/Database/SqlModel.cpp \
    ../../src/Database/SqlQuery.cpp \
    ../../src/Models/PassEntry.cpp \
    ../../src/Models/PassEntryService.cpp \
    ../../src/Models/PassGroup.cpp \
    ../../src/Models/PassGroupService.cpp \
    ../TestSupport/ApplicationSettingsStub.cpp \
    test_service.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
