SOURCE_DIR=`cd "$(dirname "$0")" && pwd`
BUILD_DIR=${BUILD_DIR:-${SOURCE_DIR}/build}
mkdir $BUILD_DIR
rm -fr $BUILD_DIR/*

cd $SOURCE_DIR
cmake -B $BUILD_DIR

cd $BUILD_DIR
make -j4