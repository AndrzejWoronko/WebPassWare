#!/bin/bash

#set -x

function replace_str()
{
#$1-search_string $2-replace_string $3-filename
old_parm=$1
new_parm=$2
filename=$3
sed -i "s/${old_parm}/${new_parm}/" ${filename}
}

WEBPASSWARE_VERSION=`grep WEBPASSWARE_VERSION CURRENT_VERSION.TXT | cut -d '=' -f 2-2`
FINAL_EXECUTABLE="bin\/webpassware"
WIZARD_STYLE="Modern"
TARGET_DIR="\/opt\/WebPassWare\/"
UPDATE_ARCH="linux"

QT_VERSION="5.15.2"
DATE=`date +%Y-%m-%d`


REQURED_QT_DIRS="lib/Qt"

REQURED_WEBPASSWARE_DIRS="bin"

OS="linux"

BIN_CREATOR_DIR="/opt/Qt/Tools/QtInstallerFramework/4.2/bin"

PWD=`pwd`

CONFIG_DIR=$PWD/installer/
OUTPUT=$PWD/output
INSTALL_DIR=$OUTPUT/webpassware-installer

cd $OUTPUT

#Delete installer dir
rm -rf $INSTALL_DIR/

mkdir -p $INSTALL_DIR/
mkdir -p $INSTALL_DIR/config/
mkdir -p $INSTALL_DIR/packages/
mkdir -p $INSTALL_DIR/packages/io.qt/
mkdir -p $INSTALL_DIR/packages/io.qt/data/
mkdir -p $INSTALL_DIR/packages/io.qt/data/lib/
mkdir -p $INSTALL_DIR/packages/io.qt/meta/
mkdir -p $INSTALL_DIR/packages/pl.com.awsoftware.webpassware/
mkdir -p $INSTALL_DIR/packages/pl.com.awsoftware.webpassware/data/
mkdir -p $INSTALL_DIR/packages/pl.com.awsoftware.webpassware/meta/

if [ ! -d $OUTPUT/portable/WebPassWare ] ; then
   echo "Required portable relase doesn't exist:  $OUTPUT/portable/WebPassWare"
   exit;
fi

SOURCE_DIR="$OUTPUT/portable/WebPassWare"

echo "Install dir: $CONFIG_DIR"

#Copy config
cp -a $CONFIG_DIR/config/*.png $INSTALL_DIR/config/
cp -a $CONFIG_DIR/config/*.ico $INSTALL_DIR/config/
cp -a $CONFIG_DIR/config/config.xml $INSTALL_DIR/config/
cp -a $CONFIG_DIR/config/controller.qs $INSTALL_DIR/config/
cp -a $CONFIG_DIR/config/style.qss $INSTALL_DIR/config/

#Copy QT package config 
cp -a $CONFIG_DIR/config/qt_package.xml $INSTALL_DIR/packages/io.qt/meta/package.xml
#Copy webpassware package config
cp -a $CONFIG_DIR/config/webpassware_package.xml $INSTALL_DIR/packages/pl.com.awsoftware.webpassware/meta/package.xml
cp -a $CONFIG_DIR/config/installscript.qs $INSTALL_DIR/packages/pl.com.awsoftware.webpassware/meta/
cp -a $CONFIG_DIR/config/create_links.ui $INSTALL_DIR/packages/pl.com.awsoftware.webpassware/meta/

#Copy data
cd $INSTALL_DIR/packages/

echo "Source dir: $SOURCE_DIR"

# Copy Qt dirs
for QDIR in $REQURED_QT_DIRS; do
  if [ ! -d $SOURCE_DIR/$QDIR ]; then
    echo "Required Qt dir doesn't exist: $SOURCE_DIR/$QDIR"
    exit 1
  fi
  cp -R $SOURCE_DIR/$QDIR io.qt/data/lib/Qt/
done

for MDIR in $REQURED_WEBPASSWARE_DIRS; do
  if [ ! -d $SOURCE_DIR/$MDIR ]; then
    echo "Required webpassware dir doesn't exist: $SOURCE_DIR/$MDIR"
    exit 1
  fi
  cp -R $SOURCE_DIR/$MDIR pl.com.awsoftware.webpassware/data/

done

#copy icons
cp -a $CONFIG_DIR/config/webpassware.png pl.com.awsoftware.webpassware/data/
cp -a $CONFIG_DIR/config/webpassware.ico pl.com.awsoftware.webpassware/data/

cd $INSTALL_DIR/

#replace  in config/config.xml

replace_str "%WEBPASSWARE_VERSION%" $WEBPASSWARE_VERSION config/config.xml
replace_str "%FINAL_EXECUTABLE%" $FINAL_EXECUTABLE config/config.xml
replace_str "%WIZARD_STYLE%" $WIZARD_STYLE config/config.xml
replace_str "%TARGET_DIR%" $TARGET_DIR config/config.xml
replace_str "%UPDATE_ARCH%" $UPDATE_ARCH config/config.xml

#replace in packages/io.qt/meta/package.xml

replace_str "%QT_VERSION%" $QT_VERSION packages/io.qt/meta/package.xml
replace_str "%DATE%" $DATE packages/io.qt/meta/package.xml

#replace in packages/pl.com.awsoftware.webpassware/meta/package.xml

replace_str "%WEBPASSWARE_VERSION%" $WEBPASSWARE_VERSION packages/pl.com.awsoftware.webpassware/meta/package.xml
replace_str "%DATE%" $DATE packages/pl.com.awsoftware.webpassware/meta/package.xml

#Create binary instalator
echo "Create binary instalator"
$BIN_CREATOR_DIR/binarycreator -f -c config/config.xml -p packages InstallWebPassWare-${WEBPASSWARE_VERSION}.bin

#Create repo
echo "Create repo"
rm -rf ../REPO
#$BIN_CREATOR_DIR/repogen --update  -p packages ../REPO
$BIN_CREATOR_DIR/repogen -p packages ../REPO

