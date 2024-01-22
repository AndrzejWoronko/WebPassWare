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
    src/Common/AutoType.cpp \
    src/Common/AutoTypeAction.cpp \
    src/Common/AutoTypePlatform.cpp \
    src/Common/AutoTypePlatformLinux.cpp \
    src/Common/AutoTypePlatformMacos.cpp \
    src/Common/AutoTypePlatformTest.cpp \
    src/Common/AutoTypePlatformWindows.cpp \
    src/Common/CsvModel.cpp \
    src/Common/DialogState.cpp \
    src/Common/ExportCsv.cpp \
    src/Common/ImportCsv.cpp \
    src/Common/Logger.cpp \
    src/Common/MaintenanceTool.cpp \
    src/Common/PasswordGenerator.cpp \
    src/Common/SplitterState.cpp \
    src/Common/SplitterStateManager.cpp \
    src/Common/TabMainWindowState.cpp \
    src/Common/TableState.cpp \
    src/Common/Tools.cpp \
    src/Database/Database.cpp \
    src/Database/Exception.cpp \
    src/Database/ExceptionSql.cpp \
    src/Database/OrmObject.cpp \
    src/Database/OrmService.cpp \
    src/Database/Service.cpp \
    src/Database/SqlField.cpp \
    src/Database/SqlModel.cpp \
    src/Database/SqlQuery.cpp \
    src/Gui/AbstractFormDialog.cpp \
    src/Gui/AbstractFormDialogController.cpp \
    src/Gui/AbstractMainWindow.cpp \
    src/Gui/Action.cpp \
    src/Gui/Dialogs/AboutDialog.cpp \
    src/Gui/Dialogs/AbstractCloseDialog.cpp \
    src/Gui/Dialogs/CsvExportDialog.cpp \
    src/Gui/Dialogs/CsvExportDialogController.cpp \
    src/Gui/Dialogs/CsvImportDialog.cpp \
    src/Gui/Dialogs/CsvImportDialogController.cpp \
    src/Gui/Dialogs/Dialog.cpp \
    src/Gui/Dialogs/FileDialog.cpp \
    src/Gui/Dialogs/LoginDialog.cpp \
    src/Gui/Dialogs/PasswordGenerateDialog.cpp \
    src/Gui/Forms/AbstractFormWidget.cpp \
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
    src/Gui/ModelForms/PassEntryDialog.cpp \
    src/Gui/ModelForms/PassEntryDialogController.cpp \
    src/Gui/ModelForms/PassGroupDialog.cpp \
    src/Gui/ModelForms/PassGroupDialogController.cpp \
    src/Gui/Settings/AbstractSettingsItemController.cpp \
    src/Gui/Settings/AbstractSettingsItemView.cpp \
    src/Gui/Settings/SettingsDatabaseView.cpp \
    src/Gui/Settings/SettingsDatabaseViewController.cpp \
    src/Gui/Settings/SettingsDialog.cpp \
    src/Gui/Settings/SettingsLookController.cpp \
    src/Gui/Settings/SettingsLookView.cpp \
    src/Gui/Settings/SettingsWidget.cpp \
    src/Gui/Widgets/Button.cpp \
    src/Gui/Widgets/ButtonBox.cpp \
    src/Gui/Widgets/CalendarWidget.cpp \
    src/Gui/Widgets/DateEdit.cpp \
    src/Gui/Widgets/DelayEditLine.cpp \
    src/Gui/Widgets/FixedLineEdit.cpp \
    src/Gui/Widgets/FontEdit.cpp \
    src/Gui/Widgets/Frame.cpp \
    src/Gui/Layouts.cpp \
    src/Gui/Dialogs/MessageBox.cpp \
    src/Gui/Splitter.cpp \
    src/Gui/Style.cpp \
    src/Gui/Widgets/Label.cpp \
    src/Gui/Widgets/ListWidget.cpp \
    src/Gui/Widgets/ListWidgetItem.cpp \
    src/Gui/Widgets/PasswordGeneratorWidget.cpp \
    src/Gui/Widgets/TabWidget.cpp \
    src/Gui/Widgets/TableView.cpp \
    src/Gui/Widgets/TableViewItemsDelegates.cpp \
    src/Gui/Widgets/ToolButton.cpp \
    src/Models/ModelTableCheck.cpp \
    src/Models/PassEntry.cpp \
    src/Models/PassEntryService.cpp \
    src/Models/PassGroup.cpp \
    src/Models/PassGroupService.cpp \
    src/WebPassWareMainWindow.cpp \
    src/main.cpp \

HEADERS += \
    src/Application.h \
    src/ApplicationSettings.h \
    src/Common/AbstractDialogStateManager.h \
    src/Common/AbstractSplitterStateManager.h \
    src/Common/AbstractStateManager.h \
    src/Common/AbstractTabMainWindowStateManager.h \
    src/Common/AbstractTableStateManager.h \
    src/Common/AutoType.h \
    src/Common/AutoTypeAction.h \
    src/Common/AutoTypePlatform.h \
    src/Common/AutoTypePlatformLinux.h \
    src/Common/AutoTypePlatformMacos.h \
    src/Common/AutoTypePlatformTest.h \
    src/Common/AutoTypePlatformWindows.h \
    src/Common/CsvModel.h \
    src/Common/DialogState.h \
    src/Common/ExportCsv.h \
    src/Common/Global.h \
    src/Common/ImportCsv.h \
    src/Common/Logger.h \
    src/Common/MaintenanceTool.h \
    src/Common/PasswordGenerator.h \
    src/Common/Singleton.h \
    src/Common/SplitterState.h \
    src/Common/SplitterStateManager.h \
    src/Common/TabMainWindowState.h \
    src/Common/TableState.h \
    src/Common/Tools.h \
    src/Database/Database.h \
    src/Database/Exception.h \
    src/Database/ExceptionSql.h \
    src/Database/OrmObject.h \
    src/Database/OrmService.h \
    src/Database/Service.h \
    src/Database/SqlField.h \
    src/Database/SqlModel.h \
    src/Database/SqlQuery.h \
    src/Gui/AbstractFormDialog.h \
    src/Gui/AbstractFormDialogController.h \
    src/Gui/AbstractMainWindow.h \
    src/Gui/Action.h \
    src/Gui/Dialogs/AboutDialog.h \
    src/Gui/Dialogs/AbstractCloseDialog.h \
    src/Gui/Dialogs/CsvExportDialog.h \
    src/Gui/Dialogs/CsvExportDialogController.h \
    src/Gui/Dialogs/CsvImportDialog.h \
    src/Gui/Dialogs/CsvImportDialogController.h \
    src/Gui/Dialogs/Dialog.h \
    src/Gui/Dialogs/FileDialog.h \
    src/Gui/Dialogs/LoginDialog.h \
    src/Gui/Dialogs/PasswordGenerateDialog.h \
    src/Gui/Forms/AbstractFormWidget.h \
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
    src/Gui/ModelForms/PassEntryDialog.h \
    src/Gui/ModelForms/PassEntryDialogController.h \
    src/Gui/ModelForms/PassGroupDialog.h \
    src/Gui/ModelForms/PassGroupDialogController.h \
    src/Gui/Settings/AbstractSettingsItemController.h \
    src/Gui/Settings/AbstractSettingsItemView.h \
    src/Gui/Settings/SettingsDatabaseView.h \
    src/Gui/Settings/SettingsDatabaseViewController.h \
    src/Gui/Settings/SettingsDialog.h \
    src/Gui/Settings/SettingsLookController.h \
    src/Gui/Settings/SettingsLookView.h \
    src/Gui/Settings/SettingsWidget.h \
    src/Gui/Widgets/Button.h \
    src/Gui/Widgets/ButtonBox.h \
    src/Gui/Widgets/CalendarWidget.h \
    src/Gui/Widgets/DateEdit.h \
    src/Gui/Widgets/DelayEditLine.h \
    src/Gui/Widgets/FixedLineEdit.h \
    src/Gui/Widgets/FontEdit.h \
    src/Gui/Widgets/Frame.h \
    src/Gui/Layouts.h \
    src/Gui/Dialogs/MessageBox.h \
    src/Gui/Splitter.h \
    src/Gui/Style.h \
    src/Gui/Widgets/Label.h \
    src/Gui/Widgets/ListWidget.h \
    src/Gui/Widgets/ListWidgetItem.h \
    src/Gui/Widgets/PasswordGeneratorWidget.h \
    src/Gui/Widgets/TabWidget.h \
    src/Gui/Widgets/TableView.h \
    src/Gui/Widgets/TableViewItemsDelegates.h \
    src/Gui/Widgets/ToolButton.h \
    src/Models/ModelTableCheck.h \
    src/Models/PassEntry.h \
    src/Models/PassEntryService.h \
    src/Models/PassGroup.h \
    src/Models/PassGroupService.h \
    src/WebPassWareMainWindow.h


FORMS += \


RESOURCES += style.qrc \
             icons.qrc


INCLUDEPATH += src/ src/Common/ src/Gui/ src/Gui/Widgets/ src/Gui/Dialogs/ \
               src/Gui/Forms/ src/Database/ src/Models/ src/Gui/ModelForms/ \
               src/Gui/Settings/

DEPENDPATH  += $${INCLUDEPATH}

DEST_DIR = $${DEST_BINS}


macx {
    CONFIG  += app_bundle
}

win32 {
    LIBS += -L$${DEST_LIBS} -lpsapi
    EXTRA_DIR ~= s,/,\\,g
    DEST_DIR ~= s,/,\\,g

    DESTDIR = $$DEST_DIR
    RC_FILE += mainicon.rc
}
unix {
    DESTDIR = $$DEST_DIR
}

QMAKE_CXXFLAGS += -Wall -Wextra -std=c++14

QMAKE_LFLAGS_RPATH += \

DISTFILES += TODO README.md
