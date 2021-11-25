#!/bin/bash

#set -x

QT_VERSION="5.15.2"
QMAKE_DIR="/opt/Qt/${QT_VERSION}/gcc_64/bin/"
QMAKE="/opt/Qt/${QT_VERSION}/gcc_64/bin/qmake"
PRO_NAME="WebPassWare"

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}

cdir=`pwd`
cpu_cores=`nproc`

mkdir -p output/build/bin/

cd output/build/
${QMAKE} ../../${PRO_NAME}.pro
make -j ${cpu_cores}

cd ${cdir}
