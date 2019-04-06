

#include <iostream>
#include "../../publisher.h"
#include "../messages.h"
#include "../utils.h"

using namespace std;


Publisher pub(8000);



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

void send_msg(string target_robot, string msg){
	// publihser send
	cout << "Robot:    " << target_robot << endl;
	cout << "Message:  " << msg << endl << endl;

	pub.publish(msg);

	cout << endl << "# Message sent! #" << endl;
}




int main(int argc, char const *argv[])
{	
	// read YAML file with params to define the robot
	// can start by getting them as arguments

	// default params
	string command = "";
	float value = NULL;
	string target_robot = "";
	string addMore = "";
	bool keep_adding = true;

	string msg = "";

	struct target_robots{
		string ALL = "all";
		string R1 = "cucumber";
		string R2 = "potato";
		string R3 = "broccoli";
	};



	

	//Robot robot;

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

	
	return 0;
}