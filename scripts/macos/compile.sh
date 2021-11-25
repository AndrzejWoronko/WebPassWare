#!/bin/bash

#set -x

QT_VERSION="5.15.2"
QMAKE_DIR="~/Qt/${QT_VERSION}/clang_64/bin/"
QMAKE="~/Qt/${QT_VERSION}/clang_64/bin/qmake"
PRO_NAME="WebPassWare"

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}

cdir=`pwd`
cpu_cores=`sysctl -n hw.logicalcpu`

mkdir -p output/build/bin/

cd output/build/
${QMAKE} ../../${PRO_NAME}.pro
make -j ${cpu_cores}

cd ${cdir}
