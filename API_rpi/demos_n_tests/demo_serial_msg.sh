#!/bin/bash

# compile
#g++ -O -W -Wall -pedantic -o demo_serial_msg -p ../comm/comm_serial.cpp ../comm/comm_msg.cpp demo_serial_msg.cpp
g++ -O -W -Wall -pedantic -o demo_serial_msg -p ../comm_rpi.cpp demo_serial_msg.cpp

# move to executables folder and run
mv `pwd`/demo_serial_msg `pwd`/executables/demo_serial_msg
mv `pwd`/gmon.out `pwd`/executables/gmon.out
`pwd`/executables/demo_serial_msg

# remove extra files created
# -> can't do it with the demo because there is a while true
# That is why we save the executables in that folder
#rm gmon.out
#rm demo_serial_msg