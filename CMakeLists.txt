cmake_minimum_required(VERSION 3.21)

project(WebPassWare VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(WPW_BUILD_TESTS "Build the unit tests" ON)
option(WPW_COVERAGE "Instrument the build for gcov/lcov coverage" OFF)

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Gui Network Sql Svg Widgets)
qt_standard_project_setup(REQUIRES 6.12)

# Where the Debian package installs; the program finds the deployed Qt next
# to it (see cmake/Packaging.cmake).
set(WPW_PACKAGE_PREFIX /opt/webpassware)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

# APP_VERSION (ApplicationSettings.h) expands to this string literal.
add_compile_definitions(WEBPASSWARE_VERSION_STR="${PROJECT_VERSION}")
file(WRITE ${CMAKE_SOURCE_DIR}/CURRENT_VERSION.TXT "WEBPASSWARE_VERSION=${PROJECT_VERSION}\n")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(WPW_COVERAGE)
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif()
endif()

# The include directories the sources rely on ("Database.h", not
# "Database/Database.h"). The core sees only its own directories, so a GUI
# header included there fails to compile.
set(WPW_CORE_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Common
    ${CMAKE_SOURCE_DIR}/src/Database
    ${CMAKE_SOURCE_DIR}/src/Models
    ${CMAKE_SOURCE_DIR}/src/Security
)
set(WPW_GUI_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src/Gui
    ${CMAKE_SOURCE_DIR}/src/Gui/Widgets
    ${CMAKE_SOURCE_DIR}/src/Gui/Dialogs
    ${CMAKE_SOURCE_DIR}/src/Gui/Forms
    ${CMAKE_SOURCE_DIR}/src/Gui/ModelForms
    ${CMAKE_SOURCE_DIR}/src/Gui/Settings
)

# Data layer and logic without Qt Widgets: database, models, CSV, password
# generator. The GUI and the tests link it.
qt_add_library(wpw_core STATIC
    src/Common/CsvCodec.cpp
    src/Common/CsvCodec.h
    src/Common/CsvFormat.cpp
    src/Common/CsvFormat.h
    src/Common/CsvModel.cpp
    src/Common/CsvModel.h
    src/Common/ExportCsv.cpp
    src/Common/ExportCsv.h
    src/Common/GenericFactory.h
    src/Common/Global.h
    src/Common/ImportCsv.cpp
    src/Common/ImportCsv.h
    src/Common/Logger.cpp
    src/Common/Logger.h
    src/Common/PasswordGenerator.cpp
    src/Common/PasswordGenerator.h
    src/Common/Singleton.h
    src/Common/Tools.cpp
    src/Common/Tools.h
    src/Database/BusyIndicator.cpp
    src/Database/BusyIndicator.h
    src/Database/Database.cpp
    src/Database/Database.h
    src/Database/Exception.cpp
    src/Database/Exception.h
    src/Database/ExceptionSql.cpp
    src/Database/ExceptionSql.h
    src/Database/OrmObject.cpp
    src/Database/OrmObject.h
    src/Database/OrmRepository.h
    src/Database/OrmService.cpp
    src/Database/OrmService.h
    src/Database/Service.cpp
    src/Database/Service.h
    src/Database/SqlField.cpp
    src/Database/SqlField.h
    src/Database/SqlModel.cpp
    src/Database/SqlModel.h
    src/Database/SqlQuery.cpp
    src/Database/SqlQuery.h
    src/Database/TransactionGuard.cpp
    src/Database/TransactionGuard.h
    src/Models/ModelTableCheck.cpp
    src/Models/ModelTableCheck.h
    src/Models/PassEntry.cpp
    src/Models/PassEntry.h
    src/Models/PassEntryService.cpp
    src/Models/PassEntryService.h
    src/Models/PassGroup.cpp
    src/Models/PassGroup.h
    src/Models/PassGroupService.cpp
    src/Models/PassGroupService.h
    src/Models/Schema.cpp
    src/Models/Schema.h
    src/Security/MasterPassword.cpp
    src/Security/MasterPassword.h
)
target_include_directories(wpw_core PUBLIC ${WPW_CORE_INCLUDE_DIRS})
# Network only for QPasswordDigestor (PBKDF2).
target_link_libraries(wpw_core PUBLIC Qt6::Core Qt6::Network Qt6::Sql)
if(WIN32)
    target_link_libraries(wpw_core PUBLIC psapi)
endif()

# The windows, dialogs and auto-type, as a library so the tests can drive them.
qt_add_library(wpw_gui STATIC
    src/Application.cpp
    src/Application.h
    src/ApplicationSettings.cpp
    src/ApplicationSettings.h
    src/Common/AbstractDialogStateManager.cpp
    src/Common/AbstractDialogStateManager.h
    src/Common/AbstractSplitterStateManager.cpp
    src/Common/AbstractSplitterStateManager.h
    src/Common/AbstractStateManager.h
    src/Common/AutoType.cpp
    src/Common/AutoType.h
    src/Common/AutoTypeAction.cpp
    src/Common/AutoTypeAction.h
    src/Common/AutoTypePlatform.cpp
    src/Common/AutoTypePlatform.h
    src/Common/AutoTypePlatformTest.cpp
    src/Common/AutoTypePlatformTest.h
    src/Common/DialogState.cpp
    src/Common/DialogState.h
    src/Common/MaintenanceTool.cpp
    src/Common/MaintenanceTool.h
    src/Common/SplitterState.cpp
    src/Common/SplitterState.h
    src/Gui/AbstractFormDialog.cpp
    src/Gui/AbstractFormDialog.h
    src/Gui/AbstractFormDialogController.cpp
    src/Gui/AbstractFormDialogController.h
    src/Gui/AbstractMainWindow.cpp
    src/Gui/AbstractMainWindow.h
    src/Gui/Action.cpp
    src/Gui/Action.h
    src/Gui/Dialogs/AboutDialog.cpp
    src/Gui/Dialogs/AboutDialog.h
    src/Gui/Dialogs/AbstractCloseDialog.cpp
    src/Gui/Dialogs/AbstractCloseDialog.h
    src/Gui/Dialogs/CsvExportDialog.cpp
    src/Gui/Dialogs/CsvExportDialog.h
    src/Gui/Dialogs/CsvExportDialogController.cpp
    src/Gui/Dialogs/CsvExportDialogController.h
    src/Gui/Dialogs/CsvImportDialog.cpp
    src/Gui/Dialogs/CsvImportDialog.h
    src/Gui/Dialogs/CsvImportDialogController.cpp
    src/Gui/Dialogs/CsvImportDialogController.h
    src/Gui/Dialogs/Dialog.cpp
    src/Gui/Dialogs/Dialog.h
    src/Gui/Dialogs/FileDialog.cpp
    src/Gui/Dialogs/FileDialog.h
    src/Gui/Dialogs/LoginDialog.cpp
    src/Gui/Dialogs/LoginDialog.h
    src/Gui/Dialogs/MessageBox.cpp
    src/Gui/Dialogs/MessageBox.h
    src/Gui/Dialogs/PasswordGenerateDialog.cpp
    src/Gui/Dialogs/PasswordGenerateDialog.h
    src/Gui/Forms/AbstractFormWidget.cpp
    src/Gui/Forms/AbstractFormWidget.h
    src/Gui/Forms/Form.cpp
    src/Gui/Forms/Form.h
    src/Gui/Forms/FormAbstractField.cpp
    src/Gui/Forms/FormAbstractField.h
    src/Gui/Forms/FormButtonField.cpp
    src/Gui/Forms/FormButtonField.h
    src/Gui/Forms/FormCheckField.cpp
    src/Gui/Forms/FormCheckField.h
    src/Gui/Forms/FormDateField.cpp
    src/Gui/Forms/FormDateField.h
    src/Gui/Forms/FormDateTimeField.cpp
    src/Gui/Forms/FormDateTimeField.h
    src/Gui/Forms/FormEditorField.cpp
    src/Gui/Forms/FormEditorField.h
    src/Gui/Forms/FormMonthField.cpp
    src/Gui/Forms/FormMonthField.h
    src/Gui/Forms/FormNumberField.cpp
    src/Gui/Forms/FormNumberField.h
    src/Gui/Forms/FormRadioButtonField.cpp
    src/Gui/Forms/FormRadioButtonField.h
    src/Gui/Forms/FormSimpleChoiceField.cpp
    src/Gui/Forms/FormSimpleChoiceField.h
    src/Gui/Forms/FormSimpleIndexChoiceField.cpp
    src/Gui/Forms/FormSimpleIndexChoiceField.h
    src/Gui/Forms/FormTextField.cpp
    src/Gui/Forms/FormTextField.h
    src/Gui/Forms/FormToggleSwitchField.cpp
    src/Gui/Forms/FormToggleSwitchField.h
    src/Gui/Forms/FormValidator.cpp
    src/Gui/Forms/FormValidator.h
    src/Gui/Forms/FormYesNoChoiceField.cpp
    src/Gui/Forms/FormYesNoChoiceField.h
    src/Gui/Layouts.cpp
    src/Gui/Layouts.h
    src/Gui/ModelForms/PassEntryDialog.cpp
    src/Gui/ModelForms/PassEntryDialog.h
    src/Gui/ModelForms/PassEntryDialogController.cpp
    src/Gui/ModelForms/PassEntryDialogController.h
    src/Gui/ModelForms/PassGroupDialog.cpp
    src/Gui/ModelForms/PassGroupDialog.h
    src/Gui/ModelForms/PassGroupDialogController.cpp
    src/Gui/ModelForms/PassGroupDialogController.h
    src/Gui/Settings/AbstractSettingsItemController.cpp
    src/Gui/Settings/AbstractSettingsItemController.h
    src/Gui/Settings/AbstractSettingsItemView.cpp
    src/Gui/Settings/AbstractSettingsItemView.h
    src/Gui/Settings/SettingsDatabaseView.cpp
    src/Gui/Settings/SettingsDatabaseView.h
    src/Gui/Settings/SettingsDatabaseViewController.cpp
    src/Gui/Settings/SettingsDatabaseViewController.h
    src/Gui/Settings/SettingsDialog.cpp
    src/Gui/Settings/SettingsDialog.h
    src/Gui/Settings/SettingsLookController.cpp
    src/Gui/Settings/SettingsLookController.h
    src/Gui/Settings/SettingsLookView.cpp
    src/Gui/Settings/SettingsLookView.h
    src/Gui/Settings/SettingsWidget.cpp
    src/Gui/Settings/SettingsWidget.h
    src/Gui/Splitter.cpp
    src/Gui/Splitter.h
    src/Gui/Style.cpp
    src/Gui/Style.h
    src/Gui/Widgets/Button.cpp
    src/Gui/Widgets/Button.h
    src/Gui/Widgets/ButtonBox.cpp
    src/Gui/Widgets/ButtonBox.h
    src/Gui/Widgets/CalendarWidget.cpp
    src/Gui/Widgets/CalendarWidget.h
    src/Gui/Widgets/DateEdit.cpp
    src/Gui/Widgets/DateEdit.h
    src/Gui/Widgets/DelayEditLine.cpp
    src/Gui/Widgets/DelayEditLine.h
    src/Gui/Widgets/FixedLineEdit.cpp
    src/Gui/Widgets/FixedLineEdit.h
    src/Gui/Widgets/FontEdit.cpp
    src/Gui/Widgets/FontEdit.h
    src/Gui/Widgets/Frame.cpp
    src/Gui/Widgets/Frame.h
    src/Gui/Widgets/Label.cpp
    src/Gui/Widgets/Label.h
    src/Gui/Widgets/ListWidget.cpp
    src/Gui/Widgets/ListWidget.h
    src/Gui/Widgets/ListWidgetItem.cpp
    src/Gui/Widgets/ListWidgetItem.h
    src/Gui/Widgets/PasswordGeneratorWidget.cpp
    src/Gui/Widgets/PasswordGeneratorWidget.h
    src/Gui/Widgets/TabWidget.cpp
    src/Gui/Widgets/TabWidget.h
    src/Gui/Widgets/TableView.cpp
    src/Gui/Widgets/TableView.h
    src/Gui/Widgets/TableViewItemsDelegates.cpp
    src/Gui/Widgets/TableViewItemsDelegates.h
    src/Gui/Widgets/ToggleSwitch.cpp
    src/Gui/Widgets/ToggleSwitch.h
    src/Gui/Widgets/ToolButton.cpp
    src/Gui/Widgets/ToolButton.h
    src/WebPassWareMainWindow.cpp
    src/WebPassWareMainWindow.h
)
target_include_directories(wpw_gui PUBLIC ${WPW_GUI_INCLUDE_DIRS})
target_link_libraries(wpw_gui PUBLIC wpw_core Qt6::Gui Qt6::Svg Qt6::Widgets)

qt_add_executable(webpassware WIN32 MACOSX_BUNDLE
    src/main.cpp
    icons.qrc
    style.qrc
)
target_link_libraries(webpassware PRIVATE wpw_gui)

if(WIN32)
    target_sources(wpw_gui PRIVATE src/Common/AutoTypePlatformWindows.h src/Common/AutoTypePlatformWindows.cpp)
    target_sources(webpassware PRIVATE mainicon.rc)
elseif(APPLE)
    enable_language(OBJCXX)
    target_sources(wpw_gui PRIVATE
        src/Common/AutoTypePlatformMacos.h src/Common/AutoTypePlatformMacos.cpp
        src/Common/MacUtils.h src/Common/MacUtils.mm)
    target_link_libraries(wpw_gui PUBLIC "-framework AppKit")
    set(wpw_macos_icon ${CMAKE_SOURCE_DIR}/packaging/webpassware.icns)
    set_source_files_properties(${wpw_macos_icon} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    target_sources(webpassware PRIVATE ${wpw_macos_icon})
    set_target_properties(webpassware PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME WebPassWare
        MACOSX_BUNDLE_GUI_IDENTIFIER pl.com.awsoftware.WebPassWare
        MACOSX_BUNDLE_ICON_FILE webpassware.icns
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_INFO_STRING AWSoftware)
else()
    find_package(X11 REQUIRED)
    target_sources(wpw_gui PRIVATE
        src/Common/AutoTypePlatformLinux.h src/Common/AutoTypePlatformLinux.cpp
        src/Common/X11Funcs.h src/Common/X11Funcs.cpp)
    target_link_libraries(wpw_gui PUBLIC X11::X11 X11::Xtst)
endif()

if(WPW_BUILD_TESTS)
    enable_testing()
    find_package(Qt6 6.12 REQUIRED COMPONENTS Test)
    add_subdirectory(tests)
endif()

if(UNIX AND NOT APPLE)
    include(cmake/Packaging.cmake)
endif()
