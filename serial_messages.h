
#ifndef SERIAL_MESSAGES_H
#define SERIAL_MESSAGES_H


/******************
 * IMPORTANT NOTE *
 ******************

 THIS DOCUMENT IS THE BASIS TO DESCIBE THE TYPE OF INFORMATION
 EXCHANGE BETWEEN THE 2 BOARDS (Teensy (Arduino based) and
 Raspberry Pi).

 FOR THIS REASON, IF YOU CHANGE SOMETHING HERE YOU MIGHT ALSO
 DO IT IN THE FILES WHERE YOU INCLUDE IT, SUCH AS
     /API_Robot/comm_.cpp
     /API_Robot/comm_.h
     /API_rpi/comm_rpi.cpp
     /API_rpi/comm_rpi.h

 AND MAYBE OTHERS TOO.

 ******************/



// Message stiles
// might go in an other place
// @@@@ NEED TO DESIGN IT -> TO SEND STATUS MESSAGES BETWEEN 2 BOARDS @@@@
struct flags{ // info RPi send to Teensy
	// some possible examples...
	int robot_id;
    bool running;
    bool stop;
    // ...
};


// might go in an other place
struct target{ // info RPi send to Teensy
	// tartet position in robot coord. syst.
    float x;
    float y;
    float th;

    // servo setpoint
    float servo; // maybe not here?

    // option to get commands as:
    float fwd;    // move FORWARD some distance
    float tr;     // TURN certain rad or deg ---- @@@@ need to clarify

};


// might go in an other place
// @@@@ talk with Andrija if we need it or not
struct sensors{ // info coming from Teensy to RPi

	/* Sensing the environment */
    // IR sensors
    float ir1;
    float ir2;

    // IMU
    // ...


    /* Current robot position */
    // robot position in robot coord. syst.
    float x;
    float y;
    float th;
    // robot position in world coord. syst.
    float xw;
    float yw;
    float thw;
};


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



void append_float(char *& msg, int & position, float value_f){
	char * value_c = to_string(value_f).c_str();
    size_t value_len = strlen(const char *value_c);
    for (int i = 0; i < value_len; i++)
    {
    	msg[position] = value_c[i];
    	position++;
    }
}



// encode the target to send the message
char * target2msg(target new_pose, bool print_msg, bool send_only_if_updaded){
	const int MAX_LEN = 64;
	const char START = '@';
	const char DELIMITER = ',';
	const char EQUAL = '=';
	const char END = '$';

    char * msg = new char[MAX_LEN];

    msg[0] = START;
    position = 1;
    msg[position] = DELIMITER;
    msg[position+1] = 's';
    msg[position+2] = EQUAL;
    position +=3;
    append_float(msg, position, new_pose.servo);
    msg[position] = DELIMITER;
    msg[position+1] = 'x';
    msg[position+2] = EQUAL;
    position +=3;
    append_float(msg, position, new_pose.x);

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


#endif