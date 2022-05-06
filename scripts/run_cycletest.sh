#!/usr/bin/env bash

KERNEL_VERSION=$(uname -r)
NOLOAD_FILE=./data/cyclictest_noload_${KERNEL_VERSION}.txt
HIGHLOAD_FILE=./data/cyclictest_highload_${KERNEL_VERSION}.txt

# make data directory
mkdir -p ./data

# run without cpu load
sudo cyclictest -S -p 90 -m -l 100000 -q -h 500 --histfile="${NOLOAD_FILE}"

# add load
sudo taskset -c 0 /bin/ping -l 60000 -q -s 10 -f localhost &
sudo taskset -c 1 /bin/ping -l 60000 -q -s 10 -f localhost &
sudo taskset -c 2 /bin/ping -l 60000 -q -s 10 -f localhost &
sudo taskset -c 3 /bin/ping -l 60000 -q -s 10 -f localhost &

# run under load
sudo cyclictest -S -p 90 -m -l 100000 -q -h 500 --histfile="${HIGHLOAD_FILE}"

# generate plots
python3 ./scripts/plot_cyclictest.py --datafile="${NOLOAD_FILE}" --user=${AC_USERNAME}
python3 ./scripts/plot_cyclictest.py --datafile="${HIGHLOAD_FILE}" --user=${AC_USERNAME}
