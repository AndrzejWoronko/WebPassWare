#!/bin/bash

#set -x

QMAKE="/c/Qt/5.15.2/mingw81_64/bin/qmake.exe"
MAKE="/c/Qt/Tools/mingw810_64/bin/mingw32-make.exe"
PATH=${PATH}:"/c/Qt/Tools/mingw810_64/bin/"

cdir=`pwd`
cpu_cores=`nproc`

mkdir -p output/build/bin/

cd output/build/
export PATH=${PATH}
$QMAKE ../../WebPassWare.pro
$MAKE -j $cpu_cores

cd $cdir
