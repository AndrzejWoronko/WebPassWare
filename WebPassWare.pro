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
    src/Database/Database.cpp \
    src/Gui/AbstractMainWindow.cpp \
    src/Gui/Action.cpp \
    src/Gui/Dialogs/AboutDialog.cpp \
    src/Gui/Dialogs/AbstractCloseDialog.cpp \
    src/Gui/Dialogs/Dialog.cpp \
    src/Gui/Forms/Form.cpp \
    src/Gui/Forms/FormAbstractField.cpp \
    src/Gui/Forms/FormButtonField.cpp \
    src/Gui/Forms/FormCheckField.cpp \
    src/Gui/Forms/FormDateField.cpp \
    src/Gui/Forms/FormDateTimeField.cpp \
    src/Gui/Forms/FormEditorField.cpp \
    src/Gui/Forms/FormMonthField.cpp \
    src/Gui/Forms/FormNumberField.cpp \
    src/Gui/Forms/FormRadioButtonField.cpp \
    src/Gui/Forms/FormSimpleChoiceField.cpp \
    src/Gui/Forms/FormSimpleIndexChoiceField.cpp \
    src/Gui/Forms/FormTextField.cpp \
    src/Gui/Forms/FormValidator.cpp \
    src/Gui/Forms/FormYesNoChoiceField.cpp \
    src/Gui/Widgets/Button.cpp \
    src/Gui/Widgets/ButtonBox.cpp \
    src/Gui/Widgets/CalendarWidget.cpp \
    src/Gui/Widgets/DateEdit.cpp \
    src/Gui/Widgets/DelayEditLine.cpp \
    src/Gui/Widgets/FixedLineEdit.cpp \
    src/Gui/Widgets/Frame.cpp \
    src/Gui/Layouts.cpp \
    src/Gui/Dialogs/MessageBox.cpp \
    src/Gui/Splitter.cpp \
    src/Gui/Style.cpp \
    src/Gui/Widgets/TabWidget.cpp \
    src/Gui/Widgets/TableView.cpp \
    src/Gui/Widgets/ToolButton.cpp \
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
    src/Database/Database.h \
    src/Gui/AbstractMainWindow.h \
    src/Gui/Action.h \
    src/Gui/Dialogs/AboutDialog.h \
    src/Gui/Dialogs/AbstractCloseDialog.h \
    src/Gui/Dialogs/Dialog.h \
    src/Gui/Forms/Form.h \
    src/Gui/Forms/FormAbstractField.h \
    src/Gui/Forms/FormButtonField.h \
    src/Gui/Forms/FormCheckField.h \
    src/Gui/Forms/FormDateField.h \
    src/Gui/Forms/FormDateTimeField.h \
    src/Gui/Forms/FormEditorField.h \
    src/Gui/Forms/FormMonthField.h \
    src/Gui/Forms/FormNumberField.h \
    src/Gui/Forms/FormRadioButtonField.h \
    src/Gui/Forms/FormSimpleChoiceField.h \
    src/Gui/Forms/FormSimpleIndexChoiceField.h \
    src/Gui/Forms/FormTextField.h \
    src/Gui/Forms/FormValidator.h \
    src/Gui/Forms/FormYesNoChoiceField.h \
    src/Gui/Widgets/Button.h \
    src/Gui/Widgets/ButtonBox.h \
    src/Gui/Widgets/CalendarWidget.h \
    src/Gui/Widgets/DateEdit.h \
    src/Gui/Widgets/DelayEditLine.h \
    src/Gui/Widgets/FixedLineEdit.h \
    src/Gui/Widgets/Frame.h \
    src/Gui/Layouts.h \
    src/Gui/Dialogs/MessageBox.h \
    src/Gui/Splitter.h \
    src/Gui/Style.h \
    src/Gui/Widgets/TabWidget.h \
    src/Gui/Widgets/TableView.h \
    src/Gui/Widgets/ToolButton.h \
    src/WebPassWareMainWindow.h


FORMS += \


RESOURCES += style.qrc \
             icons.qrc



INCLUDEPATH += src/ src/Common/ src/Gui/ src/Gui/Widgets/ src/Gui/Dialogs/ \
               src/Gui/Forms/ src/Database/


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

