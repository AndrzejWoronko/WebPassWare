#!/bin/bash

#set -x

QMAKE_DIR=~/Qt/${QT_VERSION}/clang_64/bin/
QMAKE=~/Qt/${QT_VERSION}/clang_64/bin/qmake
PRO_NAME="WebPassWare"

cdir=`pwd`

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}

cpu_cores=`sysctl -n hw.logicalcpu`

mkdir -p output/build/bin/

cd output/build/
qmake ../../${PRO_NAME}.pro
make -j ${cpu_cores}

cd ${cdir}
