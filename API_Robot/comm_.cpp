
#include "comm_.h"


void send_msg(String msg){

	// String 2 char[]
	char c_msg[sizeof(msg)];
	for (uint i = 0; i < sizeof(msg); ++i){
		c_msg[i] = msg[i];
	}

	Serial.write(c_msg);
}


String recv_msg(){
	return Serial.readString();
}


/*
	FROM HERE COPIED FROM 'comm_msg.h'
*/


// have to go with struct target
void print_target(target new_pose){
	Serial.print("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nTarget:");
	Serial.print("\n    Servo = "); Serial.print(new_pose.servo);
    Serial.print("\n    x = "); Serial.print(new_pose.x);
    Serial.print("\n    y = "); Serial.print(new_pose.y);
    Serial.print("\n    th = "); Serial.print(new_pose.th);
    Serial.print("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}



// decode the received message into target
void msg2target(target & new_pose, String msg, bool print_msg){

    // split the message
    char str[msg.length()];
    for(uint i=0; i< msg.length(); i++){
    	str[i] = msg[i];
    }
    char * pch;
    //vector<String> words;
    int words_size = 20, count=0;
    String words[words_size];
    bool keep_reading=true, store=false;
    pch = strtok (str," ,="); //" ,.-"
    while (pch != NULL && keep_reading)
    {
        if(*pch=='@' && !store){
            store=true;
        }
        else if(*pch=='$' && store){
            keep_reading=false;
            store = false;
        }
        else if(*pch!='$' && store) {
        	words[count] = pch;
        	count++;
            //words.push_back(pch);
            if (print_msg){
            	printf ("%s\n",pch);
            }
        }
        pch = strtok (NULL, " ,="); //" ,.-"
    }

    // save it as new target pose
    const float BIG_NUM = 191919.191919;
    for (int i=0; i<count; i++){ // no need to go through all array, just the words saved
    	float val = BIG_NUM;
        if(words[i+1] == "s"){
            //val = words.at(i+1).toFloat();
            val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.servo = val;
                i++;
            }
        }
        else if(words[i+1] == "x"){
            //val = words.at(i+1).toFloat();
            val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.x = val;
                i++;
            }
        }
        else if(words[i+1] == "y"){
            //val = words.at(i+1).toFloat();
            val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.y = val;
                i++;
            }
        }
        else if(words[i+1] == "th"){
            //val = words.at(i+1).toFloat();
            val = words[i+1].toFloat();
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
void target2msg(target new_pose, String & msg, bool print_msg, bool send_only_if_updaded){

	// @@@@ 'send_only_if_updaded' this might mean that we need to keep track of the previous pose
	// NOT implemented for the moment

	String new_msg = "@";
	new_msg += ",s=" + String(new_pose.servo);
	new_msg += ",x=" + String(new_pose.x);
	new_msg += ",y=" + String(new_pose.y);
	new_msg += ",th=" + String(new_pose.th);
	new_msg += ",$";

	// update message
	msg = new_msg;

	if (print_msg) Serial.println("Message: " + msg);
}	
