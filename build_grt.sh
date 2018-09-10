#!/bin/bash

BASEDIR=$(dirname $(realpath -s "$0"))
EXTERNAL_DIR=${BASEDIR}/externAssets
BUILD_DIR=${EXTERNAL_DIR}/sources/grt_source/build
TMP_DIR=${BUILD_DIR}/tmp
PREFIX=${EXTERNAL_DIR}/grt
EXAMPLES=OFF
TOOLS=OFF
TESTS=OFF

if [ ! -d ${PREFIX} ]; then
  mkdir ${PREFIX} || exit
fi

if [ ! -d ${TMP_DIR} ]; then
  mkdir ${TMP_DIR} || exit
fi

cd ${TMP_DIR} || exit

cmake .. -DCMAKE_INSTALL_PREFIX=${PREFIX} -DBUILD_EXAMPLES=${EXAMPLES} -DBUILD_TOOLS=${TOOLS} -DBUILD_TESTS=${TESTS}

if [ ! -z $1 ]; then
    make -j${8} && make install
else
    make -j8 && make install
fi

cd -
rm -r ${TMP_DIR}
