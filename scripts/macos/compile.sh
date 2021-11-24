#!/bin/bash

set -x

QMAKE="~/Qt/5.15.2/clang_64/bin/qmake"

cdir=`pwd`
cpu_cores=`sysctl -n hw.logicalcpu`

mkdir -p output/build/bin/

cd output/build/
$QMAKE ../../WebPassWare.pro
make -j $cpu_cores

cd $cdir
