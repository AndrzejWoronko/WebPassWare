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
FINAL_EXECUTABLE="bin\/webpassware.exe"
WIZARD_STYLE="Modern"
TARGET_DIR="C:\/WebPassWare\/"
UPDATE_ARCH="windows"
QT_VERSION="5.15.2"
DATE=`date +%Y-%m-%d`
IFW_PATH=/c/Qt/Tools/QtInstallerFramework/4.2/bin

echo "Settings PATH"
export PATH=${PATH}:${IFW_PATH}

PWD=`pwd`

CONFIG_DIR=$PWD/installer/
OUTPUT=$PWD/output
INSTALL_DIR=$OUTPUT/webpassware-installer

cd $OUTPUT

#Delete installer dir
rm -rf $INSTALL_DIR/

mkdir -p $INSTALL_DIR/
mkdir -p $INSTALL_DIR/config/
mkdir -p $INSTALL_DIR/packages/io.qt/data/bin/
mkdir -p $INSTALL_DIR/packages/io.qt/meta/
mkdir -p $INSTALL_DIR/packages/pl.com.awsoftware.webpassware/data/bin/
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

echo "Copu Qt libs and plugins"

cp -R $SOURCE_DIR/bin/ io.qt/data/

echo "Moving binary"
mv io.qt/data/bin/*.exe pl.com.awsoftware.webpassware/data/bin/

echo "Copy icons"

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
binarycreator.exe -f -c config/config.xml -p packages InstallWebPassWare-${WEBPASSWARE_VERSION}.bin

#Create repo
echo "Create repo"
rm -rf ../REPO
#repogen.exe --update  -p packages ../REPO
repogen.exe -p packages ../REPO
