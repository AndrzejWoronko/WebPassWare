#!/bin/bash

function replace_str()
{
#$1-search_string $2-replace_string $3-filename
old_parm=$1
new_parm=$2
filename=$3
sed -i "s/${old_parm}/${new_parm}/g" ${filename}
}

set -x

echo "Settings variables"
QT_VERSION="5.15.2"
PROGRAM_VERSION=`grep WEBPASSWARE_VERSION CURRENT_VERSION.TXT | cut -d '=' -f 2-2`
QMAKE_DIR=~/Qt/${QT_VERSION}/clang_64/bin/
QMAKE=~/Qt/${QT_VERSION}/clang_64/bin/qmake
PROGRAM_NAME="webpassware"
PORTABLE_DIR="WebPassWare"
YEAR=`date +%Y`

cdir=`pwd`

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}

OUTPUT=`pwd`/output

cd $OUTPUT

# Create portable dir to store distribution in
rm -rf portable
mkdir portable
cd portable
portable=`pwd`

# Copy all output from compilation here
echo "Prepare files to package ..."

mkdir -p ${PORTABLE_DIR}/

cp -R ${OUTPUT}/build/bin/* ${PORTABLE_DIR}/

cp -a ../../installer/config/${PROGRAM_NAME}.ico ${PORTABLE_DIR}/
cp -a ../../installer/config/${PROGRAM_NAME}.png ${PORTABLE_DIR}/

cd ${PORTABLE_DIR}/

macdeployqt ${PROGRAM_NAME}.app

#Change Info.plist
cp -a ../../../installer/config/${PROGRAM_NAME}.icns ${PROGRAM_NAME}.app/Contents/Resources/
cp -a ../../../installer/config/Info.plist ${PROGRAM_NAME}.app/Contents/

#replace in Info.plist
replace_str "%PROGRAM_VERSION%" ${PROGRAM_VERSION} ${PROGRAM_NAME}.app/Contents/Info.plist
replace_str "%ICON_NAME%" ${PROGRAM_NAME} ${PROGRAM_NAME}.app/Contents/Info.plist
replace_str "%YEAR%" ${YEAR} ${PROGRAM_NAME}.app/Contents/Info.plist
replace_str "%PROGRAM_FULL_NAME%" ${PORTABLE_DIR} ${PROGRAM_NAME}.app/Contents/Info.plist

cd ${portable}

# Complete
echo "Building complete package: ${PORTABLE_DIR}-${PROGRAM_VERSION}.tgz"
tar czf ${PORTABLE_DIR}-${PROGRAM_VERSION}.tgz ${PORTABLE_DIR}
#xz -z  ${PORTABLE_DIR}-${PROGRAM_VERSION}.tar
  
echo "Done."

echo "Portable distribution created at: $portable/${PORTABLE_DIR}-${PROGRAM_VERSION}.tgz"
