#!/bin/bash

# compile
g++ -O -W -Wall -pedantic -o ts -p ../comm_serial.cpp tests_comm_serial.cpp

# run
`pwd`/ts

# remove extra files created
rm gmon.out
rm ts