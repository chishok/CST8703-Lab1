#!/usr/bin/env bash

TARGET_NAME=CST8703Lab1-JitteryTimers
SUBFOLDER=JitteryTimers

rm -rf ./build
cmake -DCMAKE_INSTALL_PREFIX=${HOME}/.local -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -S. -B./build
cmake --build ./build --target ${TARGET_NAME} -j 4 --

sleep 1.0

DURATION=5.0

STEP_SIZE=1000
DATAFILE=data/timeLogTs${STEP_SIZE}.dat
sudo ./build/${SUBFOLDER}/${TARGET_NAME} -s $STEP_SIZE -d $DURATION -f $DATAFILE |& tee data/output${STEP_SIZE}.txt
python3 ./scripts/plot_timing.py --datafile $DATAFILE --step_size $STEP_SIZE --user "$AC_USERNAME"

sleep 0.5

STEP_SIZE=500
DATAFILE=data/timeLogTs${STEP_SIZE}.dat
sudo ./build/${SUBFOLDER}/${TARGET_NAME} -s $STEP_SIZE -d $DURATION -f $DATAFILE |& tee data/output${STEP_SIZE}.txt
python3 ./scripts/plot_timing.py --datafile $DATAFILE --step_size $STEP_SIZE --user "$AC_USERNAME"

sleep 0.5

STEP_SIZE=100
DATAFILE=data/timeLogTs${STEP_SIZE}.dat
sudo ./build/${SUBFOLDER}/${TARGET_NAME} -s $STEP_SIZE -d $DURATION -f $DATAFILE |& tee data/output${STEP_SIZE}.txt
python3 ./scripts/plot_timing.py --datafile $DATAFILE --step_size $STEP_SIZE --user "$AC_USERNAME"
