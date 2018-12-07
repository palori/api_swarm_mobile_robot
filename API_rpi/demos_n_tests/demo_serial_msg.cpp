/*
	demo_serial_msg.cpp
*/

#include "../comm/comm_msg.h"
#include "../comm/comm_serial.h"

/*
	1. listen to serial
	2. decode message
	3. compute new target
	4. encode target
	5. send to serial
*/

int main(int argc, char const *argv[])
{	
	// initialization
	string msg = "";
	target new_pose, old_pose;

	new_pose.servo = 0.0;
    new_pose.x = 0.0;
    new_pose.y = 0.0;
    new_pose.th = 0.0;

    old_pose.servo = 0.0;
    old_pose.x = 0.0;
    old_pose.y = 0.0;
    old_pose.th = 0.0;


	int fd = serial_open();
    cout << "Serial " << fd << " open:\n";
	while(true){
		cout << "\n\n  ** while **\n    1";
    	msg = read_char_by_char(fd, true); // 1
    	cout << "\n    2";
    	msg2target(old_pose, msg, false); // 2
    	cout << "    " + msg;

    	usleep(1000); //microseconds
    	cout << "\n    3";
    	new_pose.x += 1.0; // 3
		usleep(1000); //microseconds

		cout << "\n    4";
    	target2msg(new_pose, msg, false, false); // 4
    	cout << "\n    5";
    	serial_write(fd, msg, false); // 5

    	usleep(2000000); //microseconds
    }

    // so we need to close?
	return 0;
}