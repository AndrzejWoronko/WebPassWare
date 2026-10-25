#-------------------------------------------------
#
# Form fields test project
#
#-------------------------------------------------
QT += core gui testlib widgets
CONFIG += qt console
CONFIG -= app_bundle

TARGET = test_forms

TEMPLATE = app

INCLUDEPATH += ../../src/ ../../src/Common/ ../../src/Gui/Forms/ ../../src/Gui/Widgets/

DEPENDPATH  += $${INCLUDEPATH}

HEADERS += ../../src/Gui/Forms/FormAbstractField.h \
    ../../src/Gui/Forms/FormCheckField.h \
    ../../src/Gui/Forms/FormNumberField.h \
    ../../src/Gui/Forms/FormSimpleChoiceField.h \
    ../../src/Gui/Forms/FormTextField.h \
    ../../src/Gui/Widgets/FixedLineEdit.h

SOURCES += ../../src/Gui/Forms/FormAbstractField.cpp \
    ../../src/Gui/Forms/FormCheckField.cpp \
    ../../src/Gui/Forms/FormNumberField.cpp \
    ../../src/Gui/Forms/FormSimpleChoiceField.cpp \
    ../../src/Gui/Forms/FormTextField.cpp \
    ../../src/Gui/Widgets/FixedLineEdit.cpp \
    test_forms.cpp

! include( ../test_clean.pri ) {
    error( test_forms.pro: Cannot find the test_clean.pri file! )
}
