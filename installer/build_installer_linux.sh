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

PROGRAM_VERSION=`grep WEBPASSWARE_VERSION CURRENT_VERSION.TXT | cut -d '=' -f 2-2`
FINAL_EXECUTABLE="bin\/webpassware"
WIZARD_STYLE="Modern"
TARGET_DIR="\/opt\/WebPassWare\/"
UPDATE_ARCH="linux"
QT_VERSION="5.15.2"
IWF_VERSION="4.2"
IFW_PATH="/opt/Qt/Tools/QtInstallerFramework/${IWF_VERSION}/bin"
DATE=`date +%Y-%m-%d`
PROGRAM_NAME="webpassware"
PORTABLE_DIR="WebPassWare"

REQURED_QT_DIRS="lib/Qt"

REQURED_PROGRAM_DIRS="bin"

REQURED_PROGRAM_LIBS=""

PWD=`pwd`

CONFIG_DIR=${PWD}/installer/
OUTPUT=${PWD}/output
INSTALL_DIR=${OUTPUT}/${PROGRAM_NAME}-installer

cd ${OUTPUT}

#Delete installer dir
rm -rf ${INSTALL_DIR}/

mkdir -p ${INSTALL_DIR}/
mkdir -p ${INSTALL_DIR}/config/
mkdir -p ${INSTALL_DIR}/packages/
mkdir -p ${INSTALL_DIR}/packages/io.qt/
mkdir -p ${INSTALL_DIR}/packages/io.qt/data/
mkdir -p ${INSTALL_DIR}/packages/io.qt/data/lib/
mkdir -p ${INSTALL_DIR}/packages/io.qt/meta/
mkdir -p ${INSTALL_DIR}/packages/pl.com.awsoftware.${PROGRAM_NAME}/
mkdir -p ${INSTALL_DIR}/packages/pl.com.awsoftware.${PROGRAM_NAME}/data/
mkdir -p ${INSTALL_DIR}/packages/pl.com.awsoftware.${PROGRAM_NAME}/meta/

if [ ! -d $OUTPUT/portable/${PORTABLE_DIR} ] ; then
   echo "Required portable relase doesn't exist:  $OUTPUT/portable/${PORTABLE_DIR}"
   exit;
fi

SOURCE_DIR="${OUTPUT}/portable/${PORTABLE_DIR}"

echo "Install dir: ${CONFIG_DIR}"

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

# Copy Qt dirs
for QDIR in ${REQURED_QT_DIRS}; do
  if [ ! -d ${SOURCE_DIR}/${QDIR} ]; then
    echo "Required Qt dir doesn't exist: ${SOURCE_DIR}/${QDIR}"
    exit 1
  fi
  cp -R ${SOURCE_DIR}/${QDIR} io.qt/data/lib/Qt/
done

for MDIR in ${REQURED_PROGRAM_DIRS}; do
  if [ ! -d ${SOURCE_DIR}/${MDIR} ]; then
    echo "Required program dir doesn't exist: ${SOURCE_DIR}/${MDIR}"
    exit 1
  fi
  cp -R ${SOURCE_DIR}/${MDIR} pl.com.awsoftware.${PROGRAM_NAME}/data/
done

for MLIBS in ${REQURED_PROGRAM_LIBS}; do
  if [ ! -f ${SOURCE_DIR}/${MLIBS} ]; then
    echo "Required program library file doesn't exist: ${SOURCE_DIR}/${MLIBS}"
    exit 1
  fi
  cp -P ${SOURCE_DIR}/${MLIBS} pl.com.awsoftware.${PROGRAM_NAME}/data/lib/
done

#copy icons
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
${IFW_PATH}/binarycreator -f -c config/config.xml -p packages Install${PORTABLE_DIR}-${PROGRAM_VERSION}.bin

#Create repo
echo "Create repo"
rm -rf ../REPO
#${IFW_PATH}/repogen --update  -p packages ../REPO
${IFW_PATH}/repogen -p packages ../REPO

