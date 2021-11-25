#!/bin/bash

#set -x

QT_VERSION="5.15.2"
QMAKE_DIR="/c/Qt/${QT_VERSION}/mingw81_64/bin/"
MAKE_DIR="/c/Qt/Tools/mingw810_64/bin/"
PRO_NAME="WebPassWare"

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}:${MAKE_DIR}

cdir=`pwd`
cpu_cores=`nproc`

mkdir -p output/build/bin/

cd output/build/
qmake.exe ../../${PRO_NAME}.pro
mingw32-make.exe -j ${cpu_cores}

cd ${cdir}
