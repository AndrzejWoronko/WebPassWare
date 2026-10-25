# Installation and the Debian package. The package is self-contained: Qt
# (from the Qt installation used for the build) is deployed next to the
# program under /opt/webpassware, found through RPATH $ORIGIN/../lib.
#
#   cmake --preset release && cmake --build --preset release
#   cd build/release && cpack -G DEB

include(GNUInstallDirs)

install(TARGETS webpassware RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Only the plugins the program uses: X11 (xcb), SQLite, SVG icons, input
# methods. No TLS: Qt Network is linked only for PBKDF2 (QPasswordDigestor). Of Qt's own translations the program needs only the Polish qtbase
# catalogue (dialog buttons), loaded from <prefix>/translations.
qt_generate_deploy_app_script(
    TARGET webpassware
    OUTPUT_SCRIPT wpw_deploy
    NO_TRANSLATIONS
    EXCLUDE_PLUGIN_TYPES
        qmltooling egldeviceintegrations generic networkinformation tls vectorimageformats
        wayland-decoration-client wayland-graphics-integration-client wayland-shell-integration
    EXCLUDE_PLUGINS
        qsqlibase qsqlmimer qsqlmysql qsqloci qsqlodbc qsqlpsql qpdf qtvirtualkeyboardplugin
        qgif qicns qico qjpeg qtga qtiff qwbmp qwebp
)
install(SCRIPT ${wpw_deploy})
install(FILES ${QT6_INSTALL_PREFIX}/${QT6_INSTALL_TRANSLATIONS}/qtbase_pl.qm DESTINATION translations)

install(FILES README.md LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})

# Desktop integration and the command on PATH, outside the /opt prefix.
install(FILES packaging/webpassware.desktop DESTINATION /usr/share/applications)
install(FILES packaging/webpassware.png DESTINATION /usr/share/pixmaps)
install(CODE "
    file(MAKE_DIRECTORY \"\$ENV{DESTDIR}/usr/bin\")
    file(CREATE_LINK \"${WPW_PACKAGE_PREFIX}/bin/webpassware\"
         \"\$ENV{DESTDIR}/usr/bin/webpassware\" SYMBOLIC)
")

set(CPACK_PACKAGE_NAME webpassware)
set(CPACK_PACKAGE_VENDOR "AWSoftware")
set(CPACK_PACKAGE_CONTACT "Andrzej Woronko <andrzej.woronko@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Password manager")
set(CPACK_PACKAGE_DESCRIPTION
    "Keeps passwords in groups, generates passwords, imports and exports CSV and types logins into other windows (X11).")
set(CPACK_PACKAGING_INSTALL_PREFIX ${WPW_PACKAGE_PREFIX})
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SECTION utils)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_STRIP_FILES ON)
include(CPack)
