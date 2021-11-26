#!/bin/bash

#set -x

echo "Settings variables"
QT_VERSION="5.15.2"
PROGRAM_VERSION=`grep WEBPASSWARE_VERSION CURRENT_VERSION.TXT | cut -d '=' -f 2-2`
QMAKE_DIR="/c/Qt/${QT_VERSION}/mingw81_64/bin/"
MAKE_DIR="/c/Qt/Tools/mingw810_64/bin/"
QT_LIB_PATH="/c/Qt/${QT_VERSION}/mingw81_64/"
PROGRAM_NAME=webpassware
PORTABLE_DIR=WebPassWare

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}:${MAKE_DIR}


OUTPUT=`pwd`/output

cd $OUTPUT

required_modules="Qt5Core Qt5Gui Qt5Widgets Qt5Network Qt5Sql libgcc_s_seh-1 libstdc++-6 libwinpthread-1"
  
required_plugins="iconengines/qsvgicon platforms/qwindows styles/qwindowsvistastyle \
                  sqldrivers/qsqlite sqldrivers/qsqlodbc sqldrivers/qsqlpsql \
				  imageformats/qgif imageformats/qicns imageformats/qico imageformats/qjpeg imageformats/qsvg imageformats/qtga imageformats/qtiff imageformats/qwbmp"

# Create portable dir to store distribution in
rm -rf portable
mkdir portable
cd portable
portable=`pwd`

# Copy all output from compilation here
echo "Prepare files to package ..."


mkdir -p ${PORTABLE_DIR}/bin/plugins

cp -R ${OUTPUT}/build/bin/ ${PORTABLE_DIR}/

#ikony
cp -a ../../installer/config/${PROGRAM_NAME}.ico ${PORTABLE_DIR}/
cp -a ../../installer/config/${PROGRAM_NAME}.png ${PORTABLE_DIR}/

# Make lib directory to move all *.so files (webpassware files and Qt files and dependencies)

# Copy Qt
cd ${portable}/${PORTABLE_DIR}/bin/
for module in ${required_modules}; do
  if [ ! -f ${QT_LIB_PATH}/bin/${module}.dll ]; then
    echo "Required Qt module doesn't exist: ${QT_LIB_PATH}/bin/${module}.dll"
    exit 1
  fi
  cp -P ${QT_LIB_PATH}/bin/${module}.dll .
done

# Now copy Qt plugins
cd ${portable}/${PORTABLE_DIR}/bin/plugins/

for plugin in ${required_plugins}; do
  if [ ! -f ${QT_LIB_PATH}/plugins/${plugin}.dll ]; then
    echo "Required Qt plugin doesn't exist: ${QT_LIB_PATH}/plugins/${plugin}.dll"
    exit 1
  fi
  parts=(${plugin/\// })
  mkdir ${parts[0]} 2>/dev/null
  cp -P ${QT_LIB_PATH}/plugins/${plugin}.dll ${parts[0]}
done

cd ${portable}

# Complete
echo "Building complete package: ${PORTABLE_DIR}-${PROGRAM_VERSION}.tgz"
tar.exe czf ${PORTABLE_DIR}-${PROGRAM_VERSION}.tgz ${PORTABLE_DIR}
#xz.exe -z  ${PORTABLE_DIR}-${PROGRAM_VERSION}.tar
  
echo "Done."

echo "Portable distribution created at: ${portable}/${PORTABLE_DIR}-${PROGRAM_VERSION}.tgz"
