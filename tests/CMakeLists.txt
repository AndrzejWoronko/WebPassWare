# Each test compiles the sources it needs, as the former qmake test projects
# did; the database tests replace CApplicationSettings with a stub.
function(wpw_add_test name)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBS" ${ARGN})
    qt_add_executable(${name} ${ARG_SOURCES})
    target_include_directories(${name} PRIVATE ${WPW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${name} PRIVATE Qt6::Test ${ARG_LIBS})
    target_compile_definitions(${name} PRIVATE FIXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endfunction()

set(src ${CMAKE_SOURCE_DIR}/src)

set(WPW_DB_SOURCES
    ${src}/Common/Logger.cpp
    ${src}/Common/Tools.cpp
    ${src}/Database/Database.cpp
    ${src}/Database/Exception.cpp
    ${src}/Database/ExceptionSql.cpp
    ${src}/Database/OrmObject.cpp
    ${src}/Database/Service.cpp
    ${src}/Database/SqlField.cpp
    ${src}/Database/SqlModel.h ${src}/Database/SqlModel.cpp
    ${src}/Database/SqlQuery.cpp
    ${src}/Database/OrmRepository.h
    ${src}/Models/PassEntry.h ${src}/Models/PassEntry.cpp
    ${src}/Models/PassEntryService.cpp
    ${src}/Models/PassGroup.h ${src}/Models/PassGroup.cpp
    ${src}/Models/PassGroupService.cpp
    TestSupport/ApplicationSettingsStub.cpp
)

# test_orm includes the Database sources it needs directly.
wpw_add_test(tst_orm
    SOURCES ORM_test/test_orm.cpp ${src}/Database/OrmObject.cpp TestSupport/ApplicationSettingsStub.cpp
    LIBS Qt6::Sql Qt6::Widgets)
wpw_add_test(tst_service
    SOURCES Service_test/test_service.cpp ${WPW_DB_SOURCES}
    LIBS Qt6::Sql Qt6::Widgets)
wpw_add_test(tst_legacydb
    SOURCES LegacyDb_test/test_legacydb.cpp ${WPW_DB_SOURCES}
    LIBS Qt6::Sql Qt6::Widgets)
wpw_add_test(tst_csv
    SOURCES Csv_test/test_csv.cpp
        ${src}/Common/ImportCsv.cpp ${src}/Common/ExportCsv.cpp
    LIBS Qt6::Core)
wpw_add_test(tst_forms
    SOURCES Forms_test/test_forms.cpp
        ${src}/Gui/Forms/FormAbstractField.cpp
        ${src}/Gui/Forms/FormCheckField.h ${src}/Gui/Forms/FormCheckField.cpp
        ${src}/Gui/Forms/FormNumberField.h ${src}/Gui/Forms/FormNumberField.cpp
        ${src}/Gui/Forms/FormSimpleChoiceField.h ${src}/Gui/Forms/FormSimpleChoiceField.cpp
        ${src}/Gui/Forms/FormTextField.h ${src}/Gui/Forms/FormTextField.cpp
        ${src}/Gui/Widgets/FixedLineEdit.h ${src}/Gui/Widgets/FixedLineEdit.cpp
    LIBS Qt6::Widgets)

if(UNIX AND NOT APPLE)
    # Sends real X11 key events only through the test platform, but the
    # Linux platform is compiled in and needs a display connection.
    wpw_add_test(tst_autotype
        SOURCES AutoType_test/test_autotype.cpp
            ${src}/Common/AutoType.h ${src}/Common/AutoType.cpp
            ${src}/Common/AutoTypeAction.cpp
            ${src}/Common/AutoTypePlatform.h ${src}/Common/AutoTypePlatform.cpp
            ${src}/Common/AutoTypePlatformLinux.h ${src}/Common/AutoTypePlatformLinux.cpp
            ${src}/Common/AutoTypePlatformTest.h ${src}/Common/AutoTypePlatformTest.cpp
            ${src}/Common/Tools.cpp
            ${src}/Common/X11Funcs.cpp
        LIBS Qt6::Widgets X11::X11 X11::Xtst)
    set_tests_properties(tst_autotype PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=xcb")
endif()
