#!/bin/bash

set -e

QMAKE="/opt/Qt/5.15.2/gcc_64/bin/qmake"

cdir=`pwd`
cpu_cores=`nproc`
absolute_path=`realpath $0`
this_dir=`dirname $absolute_path`
this_dir=`dirname $this_dir`
parent_dir=`dirname $this_dir`

cd $parent_dir
mkdir -p output/build/bin/

cd output/build/
$QMAKE ../../WebPassWare.pro
make -j $cpu_cores

cd $cdir
