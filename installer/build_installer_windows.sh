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

echo "Settings variables"
PROGRAM_VERSION=`grep WEBPASSWARE_VERSION CURRENT_VERSION.TXT | cut -d '=' -f 2-2`
FINAL_EXECUTABLE="bin\/webpassware.exe"
WIZARD_STYLE="Modern"
TARGET_DIR="C:\/WebPassWare\/"
UPDATE_ARCH="windows"
QT_VERSION="5.15.2"
IFW_VERSION="4.2"
IFW_PATH=/c/Qt/Tools/QtInstallerFramework/${IFW_VERSION}/bin
DATE=`date +%Y-%m-%d`
PROGRAM_NAME=webpassware
PORTABLE_DIR=WebPassWare

echo "Settings PATH"
export PATH=${PATH}:${IFW_PATH}

PWD=`pwd`

CONFIG_DIR=$PWD/installer/
OUTPUT=${PWD}/output
INSTALL_DIR=${OUTPUT}/${PROGRAM_NAME}-installer

cd ${OUTPUT}

#Delete installer dir
rm -rf $INSTALL_DIR/

rm -rf ${INSTALL_DIR}/

mkdir -p ${INSTALL_DIR}/
mkdir -p ${INSTALL_DIR}/config/
mkdir -p ${INSTALL_DIR}/packages/io.qt/data/bin/
mkdir -p ${INSTALL_DIR}/packages/io.qt/meta/
mkdir -p ${INSTALL_DIR}/packages/pl.com.awsoftware.${PROGRAM_NAME}/data/bin/
mkdir -p ${INSTALL_DIR}/packages/pl.com.awsoftware.${PROGRAM_NAME}/meta/

if [ ! -d $OUTPUT/portable/${PORTABLE_DIR} ] ; then
   echo "Required portable relase doesn't exist:  $OUTPUT/portable/${PORTABLE_DIR}"
   exit;
fi

SOURCE_DIR="${OUTPUT}/portable/${PORTABLE_DIR}"

echo "Install dir: ${CONFIG_DIR}"

#Copy config
cp -a $CONFIG_DIR/config/*.png $INSTALL_DIR/config/
cp -a $CONFIG_DIR/config/*.ico $INSTALL_DIR/config/
cp -a $CONFIG_DIR/config/config.xml $INSTALL_DIR/config/
cp -a $CONFIG_DIR/config/controller.qs $INSTALL_DIR/config/
cp -a $CONFIG_DIR/config/style.qss $INSTALL_DIR/config/


#Copy config
cp -a ${CONFIG_DIR}/config/*.png ${INSTALL_DIR}/config/
cp -a ${CONFIG_DIR}/config/*.ico ${INSTALL_DIR}/config/
cp -a ${CONFIG_DIR}/config/config.xml ${INSTALL_DIR}/config/
cp -a ${CONFIG_DIR}/config/controller.qs ${INSTALL_DIR}/config/
cp -a ${CONFIG_DIR}/config/style.qss ${INSTALL_DIR}/config/

#Copy QT package config 
cp -a ${CONFIG_DIR}/config/qt_package.xml ${INSTALL_DIR}/packages/io.qt/meta/package.xml

#Copy ${PROGRAM_NAME} package config
cp -a ${CONFIG_DIR}/config/${PROGRAM_NAME}_package.xml ${INSTALL_DIR}/packages/pl.com.awsoftware.${PROGRAM_NAME}/meta/package.xml
cp -a ${CONFIG_DIR}/config/installscript.qs ${INSTALL_DIR}/packages/pl.com.awsoftware.${PROGRAM_NAME}/meta/
cp -a ${CONFIG_DIR}/config/create_links.ui ${INSTALL_DIR}/packages/pl.com.awsoftware.${PROGRAM_NAME}/meta/

#Copy data
cd ${INSTALL_DIR}/packages/

echo "Source dir: ${SOURCE_DIR}"

echo "Copy Qt libs and plugins"

cp -R ${SOURCE_DIR}/bin/ io.qt/data/

echo "Moving binary"
mv io.qt/data/bin/*.exe pl.com.awsoftware.${PROGRAM_NAME}/data/bin/

#echo "Moving library"
#mv io.qt/data/bin/QtZint.dll pl.com.awsoftware.${PROGRAM_NAME}/data/bin/

echo "Copy icons"
cp -a ${CONFIG_DIR}/config/${PROGRAM_NAME}.png pl.com.awsoftware.${PROGRAM_NAME}/data/
cp -a ${CONFIG_DIR}/config/${PROGRAM_NAME}.ico pl.com.awsoftware.${PROGRAM_NAME}/data/

cd ${INSTALL_DIR}/

#replace  in config/config.xml

replace_str "%PROGRAM_VERSION%" ${PROGRAM_VERSION} config/config.xml
replace_str "%FINAL_EXECUTABLE%" ${FINAL_EXECUTABLE} config/config.xml
replace_str "%WIZARD_STYLE%" ${WIZARD_STYLE} config/config.xml
replace_str "%TARGET_DIR%" ${TARGET_DIR} config/config.xml
replace_str "%UPDATE_ARCH%" ${UPDATE_ARCH} config/config.xml

#replace in packages/io.qt/meta/package.xml

replace_str "%QT_VERSION%" ${QT_VERSION} packages/io.qt/meta/package.xml
replace_str "%DATE%" ${DATE} packages/io.qt/meta/package.xml

#replace in packages/pl.com.awsoftware.webpassware/meta/package.xml

replace_str "%PROGRAM_VERSION%" ${PROGRAM_VERSION} packages/pl.com.awsoftware.${PROGRAM_NAME}/meta/package.xml
replace_str "%DATE%" ${DATE} packages/pl.com.awsoftware.${PROGRAM_NAME}/meta/package.xml

#Create binary instalator
echo "Create binary instalator"
binarycreator.exe -f -c config/config.xml -p packages Install${PORTABLE_DIR}-${PROGRAM_VERSION}.bin

#Create repo
echo "Create repo"
rm -rf ../REPO
#repogen.exe --update  -p packages ../REPO
repogen.exe -p packages ../REPO
