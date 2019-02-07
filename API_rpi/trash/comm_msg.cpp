
#include "comm_msg.h"

/*
#include "pins.h"
#ifdef USING_IS_ARDUINO
#include "IS_ARDUINO.h"
#define IS_ARDUINO 1
#elif USING_RPI
#define IS_ARDUINO 0
#endif
*/

void print_flags(flags status){
    /*if (IS_ARDUINO){

    }
    else{*/
        cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nStatus flags:";
        cout << "\n    Work in progress...";
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
    //}
	
}

// have to go with struct target
void print_target(target new_pose){

    /*if (IS_ARDUINO){

    }
    else{*/
        cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nTarget:";
        cout << "\n    Servo = " << new_pose.servo;
        cout << "\n    x = " << new_pose.x;
        cout << "\n    y = " << new_pose.y;
        cout << "\n    th = " << new_pose.th;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
    //}
	
}

// have to go with struct sensors
void print_sensors(sensors new_sens){
    /*if (IS_ARDUINO){

    }
    else{*/
        cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nSensors:";
        cout << "\n    ir1 = " << new_sens.ir1;
        cout << "\n    ir2 = " << new_sens.ir2;
        cout << "\n    xw = " << new_sens.xw;
        cout << "\n    yw = " << new_sens.yw;
        cout << "\n    thw = " << new_sens.thw;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
    //}
	
}

// this should go in utils
float str2float(string s){
    try{
        return stof(s); 
    }
    catch (exception& e)
	{
	    cout << "Exception: " << e.what() << '\n';
	}
	return BIG_NUM;
}

// decode the received message into target
void msg2target(target & new_pose, string msg, bool print_msg){

    // split the message
    char str[msg.length()];
    for(unsigned int i=0; i< msg.length(); i++){
    	str[i] = msg[i];
    }
    char * pch;
    vector<string> words;
    bool keep_reading=true, store=false;
    pch = strtok (str," ,="); //" ,.-"
    while (pch != NULL && keep_reading)
    {
        //usleep(1000000); //microseconds
        
        //cout << c[0];
        if(*pch=='@' && !store){
            store=true;
        }
        else if(*pch=='$' && store){
            keep_reading=false;
            store = false;
        }
        else if(*pch!='$' && store) {
            words.push_back(pch);
            if (print_msg){
            	printf ("%s\n",pch);
            }
        }
        pch = strtok (NULL, " ,="); //" ,.-"
    }

    // save it as new target pose
    for (uint i=0; i<words.size(); i++){
        if(words.at(i) == "s"){
            float val = str2float(words.at(i+1));
            if (val != BIG_NUM) {
                new_pose.servo = val;
                i++;
            }
        }
        else if(words.at(i) == "x"){
            float val = str2float(words.at(i+1));
            if (val != BIG_NUM) {
                new_pose.x = val;
                i++;
            }
        }
        else if(words.at(i) == "y"){
            float val = str2float(words.at(i+1));
            if (val != BIG_NUM) {
                new_pose.y = val;
                i++;
            }
        }
        else if(words.at(i) == "th"){
            float val = str2float(words.at(i+1));
            if (val != BIG_NUM) {
                new_pose.th = val;
                i++;
            }
        }
    }

    if (print_msg){ // show that results are saved
    	print_target(new_pose);
    }
}


// encode the target to send the message
void target2msg(target new_pose, string & msg, bool print_msg, bool send_only_if_updaded){

	// @@@@ 'send_only_if_updaded' this might mean that we need to keep track of the previous pose
	// NOT implemented for the moment

	string new_msg = "@";
	new_msg += ",s=" + to_string(new_pose.servo);
	new_msg += ",x=" + to_string(new_pose.x);
	new_msg += ",y=" + to_string(new_pose.y);
	new_msg += ",th=" + to_string(new_pose.th);
	new_msg += ",$";

	// update message
	msg = new_msg;

	if (print_msg) cout << "Message: " << msg << endl;
}	




