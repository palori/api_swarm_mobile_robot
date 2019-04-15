

#include <iostream>
#include "../../publisher.h"
#include "../messages.h"
#include "../utils.h"

using namespace std;


Publisher pub(8001);


void send_msg(string target_robot, string msg){
	// publihser send
	pub.publish(msg);
	cout << "Robots:    " << target_robot << endl;
	cout << "Message: " << msg << endl << endl;
}


void display_help(){
	string line = "------------------------------------------------";

	cout << endl << line << endl;
	cout << "  Help" << endl << endl;
	cout << "    1. Target robot/s:" << endl;
	cout << "    - name of the robot to send the command" << endl;
	cout << "    - all (for all robots)" << endl;
	cout << endl;
	cout << "    2. Commands:" << endl;
	cout << "    - start (the mission)" << endl;
	cout << "    - pause (the mission)" << endl;
	cout << "    - fwd (distance)" << endl;
	cout << "    - v (velocity [m/s])" << endl;
	cout << "    - h (display this help menu)" << endl;
	cout << "    - Ctrl+C (exit)" << endl;
	cout << endl;
	cout << "    3. Value:" << endl;
	cout << "    - acording to the  previous command" << endl;
	cout << line << endl << endl;
}


void ui_style(){
	// default params
	string command = "";
	float value = NULL;
	string target_robot = "";
	string addMore = "";
	bool keep_adding = true;

	string msg = "";

	cout << "**************************************** " << endl;
	cout << " Welcome MASTER" << endl << endl;

	while (true){

		keep_adding = true;

		cout << endl << endl << endl << "#############################" << endl;
		cout << " Ready to send a new message" << endl << endl;

		cout << "1. Target robot/s: ";
		cin >> target_robot;

		msg = "@rob=" + target_robot;

		while (keep_adding){

			cout << "2. Command: ";
			cin >> command;

			if (command == "h") {
				display_help();
			}

			else{	// NO help

				if(command == "fwd" || command == "v") {
					cout << "3. Value: ";
					cin >> value;
					msg += "," + command + "=" + to_string(value);
					
				}
				else if (command == "a"){
					int val;
					cout << "3. Value: ";
					cin >> val;
					msg += "," + command + "=" + to_string(val) + ",b=1";
				}
				else if (command == "start"){
					msg += ",a=" + to_string(START);
				}
				else if (command == "pause"){
					msg += ",a=" + to_string(PAUSE);
				}


			
				cout << endl << "Would you like to send an other command? [y/N]" << endl;
				cin >> addMore;
				if (addMore == "y" || addMore == "Y") {
					//msg += ",";
					cout << endl << endl << "Message: " << msg << endl << endl;
				}
				else {
					keep_adding = false;
					msg += "$";

					send_msg(target_robot, msg);
				}
			}
		}
	}

	cout << "**************************************** " << endl;
}


void help2(){
	string line = "-------------------------------------------";
	cout << "\n\n" << line << "\n  HELP:\n";
	cout << "    h\t\thelp\n";
	cout << "    q\t\tquit\n";
	cout << "    command\twithout '@' and '$'\n";
	cout << "    \t\tE.g: rob=all,a=15,b=1,v=0.3,fwd=1\n";
	cout << "    \t\t     rob=all,a=16,b=1,v=0.3,trn=1.75\n";
	cout << "    \t\t     rob=all,a=2,b=1\n";
	cout << "    \t\t     rob=all,a=28\n";
	cout << "    \t\trob\tall or hostname1-hostname2-hostnameN\n";
	cout << line << endl;
}

void straight_messages(){
	string stars = "****************************************";
	string msg = "";

	cout << stars << "\n Welcome MASTER\n\n";
	cout << "\n\n start = " << START << endl;
	help2();

	int count = 0;
	bool quit = false;
	while (!quit){

		cout << "\n\n- new_msg:       ";
		cin >> msg;
		if (msg == "q") quit = true;
		else if (msg == "h") help2();
		else{								// test: rob=all,a=15,b=1,fwd=1,v=0.3
			msg = encode_master_commands(msg, count);
			pub.publish(msg);
			cout <<     "     sent:  " << msg;
			count++;
			if (count >= 10) count = 0;
		}
	}
	cout << "\n Good bye!" << endl << stars << endl;
}


int main(int argc, char const *argv[])
{	
	// read YAML file with params to define the robot
	// can start by getting them as arguments

	//ui_style();			// nice but tedious for testing
	straight_messages();	// optimised for fast testing
	
	return 0;
}