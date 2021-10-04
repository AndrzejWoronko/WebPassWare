OBJECTS_DIR = .obj
MOC_DIR = .moc
UI_DIR = .ui
RCC_DIR = .qrc

CODECFORSRC = UTF-8
CODECFORTR = UTF-8

QT += core gui sql svg
CONFIG += qt mmx sse stl exceptions warn_on c++11 silent

BUILD_DIR = .
DEST_INCLUDE_DIR = ./

DEST_LIBS = $${BUILD_DIR}/lib/
DEST_BINS = $${BUILD_DIR}/bin/

WEBPASSWARE_VERSION_MAJOR = 0
WEBPASSWARE_VERSION_MINOR = 0
WEBPASSWARE_VERSION_RELEASE = 1

WEBPASSWARE_VERSION = '\\"$${WEBPASSWARE_VERSION_MAJOR}.$${WEBPASSWARE_VERSION_MINOR}.$${WEBPASSWARE_VERSION_RELEASE}\\"'
DEFINES += WEBPASSWARE_VERSION_STR=\"$${WEBPASSWARE_VERSION}\"
DEFINES += WEBPASSWARE_VERSION=$${WEBPASSWARE_VERSION}