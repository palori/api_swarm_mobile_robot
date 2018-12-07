
#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <iostream>
#include <string.h>
#include <vector>
#include <time.h>
#include <unistd.h> // time sleep microseconds
#include <chrono> // the best one to count time in [s, ms, ns...]
//#include <Arduino.h>
using namespace std;
using namespace std::chrono;


// might go in an other place
struct target{
    float x;
    float y;
    float th;
    float servo; // maybe not here?
};

// have to go with struct target
void print_target(target new_pose){
	cout << "\n\nTarget:\n    Servo = " << new_pose.servo;
    cout << "\n    x = " << new_pose.x;
    cout << "\n    y = " << new_pose.y;
    cout << "\n    th = " << new_pose.th << endl;
}

// this should go in utils
float str2float(string s);

void msg2target(target & new_pose, string msg, bool print_msg);

string target2msg(target, bool print_msg);



