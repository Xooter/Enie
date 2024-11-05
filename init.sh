#!/bin/bash

PROJECT_DIR=$(pwd)
BUILD_DIR=${PROJECT_DIR}/build

mkdir -p $BUILD_DIR

build_mode() {
  local mode=$1
  echo "Compiling $mode..."

  cmake -S $PROJECT_DIR -B $BUILD_DIR -DCMAKE_BUILD_TYPE=$mode
  cmake --build $BUILD_DIR

  strip ${BUILD_DIR}/Enie

  echo "Finish"
}

run_binary() {
  local mode=$1
  BUILD_MODE_DIR=${BUILD_DIR}/${mode}
  echo "Running..."

  ${BUILD_DIR}/Enie /dev/input/event9
}

build_mode Release
run_binary Release
