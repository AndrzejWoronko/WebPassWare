#!/bin/bash

set -x

echo "Settings variables"
QT_VERSION="5.15.2"
PROGRAM_VERSION=`grep WEBPASSWARE_VERSION CURRENT_VERSION.TXT | cut -d '=' -f 2-2`
QMAKE_DIR="/Qt/${QT_VERSION}/clang_64/bin/"
QMAKE="/Qt/${QT_VERSION}/clang_64/bin/qmake"
PROGRAM_NAME=webpassware
PORTABLE_DIR=WebPassWare

cdir=`pwd`
cd ~/
HOME=`pwd`

cd ${cdir}

echo "Settings PATH"
export PATH=${PATH}:${HOME}/${QMAKE_DIR}

OUTPUT=`pwd`/output

cd $OUTPUT

# Create portable dir to store distribution in
rm -rf portable
mkdir portable
cd portable
portable=`pwd`

# Copy all output from compilation here
echo "Prepare files to package ..."

mkdir -p ${PORTABLE_DIR}/bin/

cp -R ${OUTPUT}/build/bin ${PORTABLE_DIR}/

cp -a ../../installer/config/${PROGRAM_NAME}.ico ${PORTABLE_DIR}/
cp -a ../../installer/config/${PROGRAM_NAME}.png ${PORTABLE_DIR}/

cd ${PORTABLE_DIR}/bin/

macdeployqt ${PROGRAM_NAME}.app

cd ${portable}

# Complete
echo "Building complete package: ${PORTABLE_DIR}-${PROGRAM_VERSION}.tgz"
tar.exe czf ${PORTABLE_DIR}-${PROGRAM_VERSION}.tgz ${PORTABLE_DIR}
#xz.exe -z  ${PORTABLE_DIR}-${PROGRAM_VERSION}.tar
  
echo "Done."

echo "Portable distribution created at: $portable/${PORTABLE_DIR}.tgz"
