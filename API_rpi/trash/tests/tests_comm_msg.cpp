
#include "../comm_msg.h"


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

void test_target2msg(){
    string msg = "";
    target new_pose;
    cout << "0. message= " << msg << endl;

    new_pose.servo = 1.0;
    new_pose.x = 2.0;
    new_pose.y = 3.0;
    new_pose.th = 4.0;
    target2msg(new_pose, msg, true, false);
    cout << "1. message= " << msg << endl;

    new_pose.servo = 11.0;
    new_pose.x = 22.0;
    new_pose.y = 33.0;
    new_pose.th = 44.0;
    target2msg(new_pose, msg, true, false);
    cout << "2. message= " << msg << endl;
}



int main(){
    test_msg2target();              //working
    test_target2msg();              //working
    return 0;
}