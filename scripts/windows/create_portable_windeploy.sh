#!/bin/bash

#set -x

QMAKE_DIR="/c/Qt/5.15.2/mingw81_64/bin/"
MAKE_DIR="/c/Qt/Tools/mingw810_64/bin/"

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}:${MAKE_DIR}

VERSION=`grep WEBPASSWARE_VERSION CURRENT_VERSION.TXT | cut -d '=' -f 2`
 
OUTPUT=`pwd`/output

cd $OUTPUT

# Create portable dir to store distribution in
rm -rf portable
mkdir portable
cd portable
portable=`pwd`

# Copy all output from compilation here
echo "Prepare files to package ..."

mkdir -p WebPassWare/bin/

cp -R $OUTPUT/build/bin/ WebPassWare/

#ikony
cp -a ../../icons/progicon.ico WebPassWare/
cp -a ../../icons/50px/WebPassWare.png WebPassWare/

cd WebPassWare/bin/

windeployqt.exe --plugindir plugins webpassware.exe

cd $portable

# Complete
echo "Building complete package: WebPassWare-$VERSION.tgz"
tar.exe czf WebPassWare-$VERSION.tgz WebPassWare
#xz -z  WebPassWare-$VERSION.tar
  
echo "Done."

echo "Portable distribution created at: $portable/WebPassWare"
