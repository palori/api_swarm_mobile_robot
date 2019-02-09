#!/bin/bash

# compile
g++ -O -W -Wall -pedantic -o head_cond -p header_conditionals.cpp test_head_cond.cpp

# run
#`pwd`/executables/demo_serial_msg

# remove extra files created
#rm gmon.out
#rm head_cond