SOURCE_DIR=`cd "$(dirname "$0")" && pwd`
cd $SOURCE_DIR

BUILD_DIR=${BUILD_DIR:-${SOURCE_DIR}/build}
mkdir $BUILD_DIR
rm -fr $BUILD_DIR/*
cmake -B $BUILD_DIR

cd $BUILD_DIR
make -j4