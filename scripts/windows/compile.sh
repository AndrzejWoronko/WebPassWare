#!/bin/bash

set -x

QMAKE="/c/Qt/5.15.2/mingw81_64/bin/qmake.exe"
MAKE="/c/Qt/Tools/mingw810_64/bin/mingw32-make.exe"
PATH=${PATH}:"/c/Qt/Tools/mingw810_64/bin/"

cdir=`pwd`
cpu_cores=`nproc`
absolute_path=`realpath $0`
this_dir=`dirname $absolute_path`
this_dir=`dirname $this_dir`
parent_dir=`dirname $this_dir`

cd $parent_dir
mkdir -p output/build/bin/

cd output/build/
export PATH=${PATH}
$QMAKE ../../WebPassWare.pro
$MAKE -j $cpu_cores

cd $cdir
