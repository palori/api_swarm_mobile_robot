
#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <iostream>
#include <string.h>
#include <vector>
#include <unistd.h> // time sleep microseconds
#include <chrono> // the best one to count time in [s, ms, ns...]
//#include <Arduino.h>

#include "comm_msg.h"
using namespace std;
using namespace std::chrono;


void test_msg2target(){
    string msg = "00000000000,@,s,x=0.22,y=0.53,th=0.08,$,0000";
    target new_pose;
    new_pose.servo = 200.0;
    new_pose.y = 11.11;
    print_target(new_pose);
    msg2target(new_pose, msg, true);
    msg = "00000000000,@,sx=10,y1,th=1.75,$,0000";
    msg2target(new_pose, msg, true);
    msg = "00000000000,,s=300,x=10,y=2,th=1.75,$,0000";
    msg2target(new_pose, msg, true);
    msg = "00000000000,@,x=10,y=3.3";
    msg2target(new_pose, msg, true);
}



int main(){
    test_msg2target();              //working
    
    return 0;
}