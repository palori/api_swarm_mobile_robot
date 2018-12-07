#!/bin/bash

# compile
g++ -O -W -Wall -pedantic -o tm -p ../comm_msg.cpp tests_comm_msg.cpp

# run
`pwd`/tm

# remove extra files created
rm gmon.out
rm tm