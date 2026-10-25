# One executable per unit, linked against wpw_core (no GUI, runs under
# QCoreApplication) or wpw_gui.
function(wpw_add_test name)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBS" ${ARGN})
    qt_add_executable(${name} ${ARG_SOURCES})
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${name} PRIVATE Qt6::Test ${ARG_LIBS})
    target_compile_definitions(${name} PRIVATE FIXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endfunction()

wpw_add_test(tst_orm SOURCES ORM_test/test_orm.cpp LIBS wpw_core)
wpw_add_test(tst_service SOURCES Service_test/test_service.cpp LIBS wpw_core)
wpw_add_test(tst_legacydb SOURCES LegacyDb_test/test_legacydb.cpp LIBS wpw_core)
wpw_add_test(tst_csv SOURCES Csv_test/test_csv.cpp LIBS wpw_core)
wpw_add_test(tst_masterpassword SOURCES MasterPassword_test/test_masterpassword.cpp LIBS wpw_core)
wpw_add_test(tst_transactionguard SOURCES TransactionGuard_test/test_transactionguard.cpp LIBS wpw_core)
wpw_add_test(tst_forms SOURCES Forms_test/test_forms.cpp LIBS wpw_gui)
wpw_add_test(tst_logindialog SOURCES LoginDialog_test/test_logindialog.cpp LIBS wpw_gui)
wpw_add_test(tst_maintenancetool SOURCES MaintenanceTool_test/test_maintenancetool.cpp LIBS wpw_gui)

if(UNIX AND NOT APPLE)
    # The Linux auto-type platform needs a display connection.
    wpw_add_test(tst_autotype SOURCES AutoType_test/test_autotype.cpp LIBS wpw_gui)
    set_tests_properties(tst_autotype PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=xcb")
endif()
