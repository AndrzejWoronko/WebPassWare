#!/bin/bash

#set -x

QT_VERSION="5.15.2"
QMAKE_DIR="/Qt/${QT_VERSION}/clang_64/bin/"
QMAKE="/Qt/${QT_VERSION}/clang_64/bin/qmake"
PRO_NAME="WebPassWare"

cdir=`pwd`
cd ~/
HOME=`pwd`

cd ${cdir}

echo "Settings PATH"
export PATH=${PATH}:${HOME}/${QMAKE_DIR}

cpu_cores=`sysctl -n hw.logicalcpu`

mkdir -p output/build/bin/

cd output/build/
qmake ../../${PRO_NAME}.pro
make -j ${cpu_cores}

cd ${cdir}
