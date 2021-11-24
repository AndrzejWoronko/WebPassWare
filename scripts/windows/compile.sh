#!/bin/bash

#set -x

QMAKE_DIR="/c/Qt/5.15.2/mingw81_64/bin/"
MAKE_DIR="/c/Qt/Tools/mingw810_64/bin/"

echo "Settings PATH"
export PATH=${PATH}:${QMAKE_DIR}:${MAKE_DIR}

cdir=`pwd`
cpu_cores=`nproc`

mkdir -p output/build/bin/

cd output/build/

qmake.exe ../../WebPassWare.pro

mingw32-make.exe -j $cpu_cores

cd $cdir
