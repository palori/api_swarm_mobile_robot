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
    //test_msg2target();              //not-working
    test_target2msg();              //not-working
    return 0;
}