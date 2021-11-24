#!/bin/bash

#set -x

QMAKE_DIR="/c/Qt/5.15.2/mingw81_64/bin/"
MAKE_DIR="/c/Qt/Tools/mingw810_64/bin/"
QT_LIB_PATH="/c/Qt/5.15.2/mingw81_64/"

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}:${MAKE_DIR}

VERSION=`grep WEBPASSWARE_VERSION CURRENT_VERSION.TXT | cut -d '=' -f 2`

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

mkdir -p WebPassWare/bin/plugins

cp -R $OUTPUT/build/bin/ WebPassWare/

#ikony
cp -a ../../icons/progicon.ico WebPassWare/
cp -a ../../icons/50px/WebPassWare.png WebPassWare/
 
# Make lib directory to move all *.so files (webpassware files and Qt files and dependencies)

# Copy Qt
cd $portable/WebPassWare/bin/
for module in $required_modules; do
  if [ ! -f $QT_LIB_PATH/bin/$module.dll ]; then
    echo "Required Qt module doesn't exist: $QT_LIB_PATH/bin/$module.dll"
    exit 1
  fi
  cp -P $QT_LIB_PATH/bin/$module.dll .
done

# Now copy Qt plugins
cd $portable/WebPassWare/bin/plugins/

for plugin in $required_plugins; do
  if [ ! -f $QT_LIB_PATH/plugins/$plugin.dll ]; then
    echo "Required Qt plugin doesn't exist: $QT_LIB_PATH/plugins/$plugin.dll"
    exit 1
  fi
  parts=(${plugin/\// })
  mkdir ${parts[0]} 2>/dev/null
  cp -P $QT_LIB_PATH/plugins/$plugin.dll ${parts[0]}
done

cd $portable

# Complete
echo "Building complete package: WebPassWare-$VERSION.tgz"
tar.exe czf WebPassWare-$VERSION.tgz WebPassWare
#xz -z  WebPassWare-$VERSION.tar
  
echo "Done."

echo "Portable distribution created at: $portable/WebPassWare"
