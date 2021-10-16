#-------------------------------------------------
#
# Project created by QtCreator 2013-03-22T23:23:58
#
#-------------------------------------------------
QT += core gui testlib network sql xml script svg
CONFIG += qt mmx sse stl exceptions warn_on

//QT       += sql testlib
//QT       -= gui

TARGET = test_orm
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src/ ../../src/Common/ ../../src/Database/ \
        ../../src/Models/

DEPENDPATH  += $${INCLUDEPATH}

HEADERS += ../../src/Database/OrmObject.cpp

SOURCES += ../../src/Database/OrmObject.cpp test_orm.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
