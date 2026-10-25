#-------------------------------------------------
#
# CSV import/export test project
#
#-------------------------------------------------
QT += core testlib
CONFIG += qt console
CONFIG -= app_bundle

TARGET = test_csv

TEMPLATE = app

INCLUDEPATH += ../../src/ ../../src/Common/

DEPENDPATH  += $${INCLUDEPATH}

HEADERS += ../../src/Common/ImportCsv.h \
    ../../src/Common/ExportCsv.h

SOURCES += ../../src/Common/ImportCsv.cpp \
    ../../src/Common/ExportCsv.cpp \
    test_csv.cpp

! include( ../test_clean.pri ) {
    error( test_csv.pro: Cannot find the test_clean.pri file! )
}
