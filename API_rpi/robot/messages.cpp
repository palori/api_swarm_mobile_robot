#include "messages.h"



string detect_message(string msg){
	string msg = "00@a=1,b=1,l=3.00$99";
	//cout << "msg = " << msg << endl;
	bool store = false, end = false;
	string msg_out = "";
	for (int i = 0; i < msg.length(); ++i){
		//cout << msg[i] << endl;
		if(msg[i] == '@'){
			store = true;
		}
		else if(store && msg[i]=='$'){
			end = true;
			store = false
			break;
		}
		else if(store){
			msg_out += msg[i];
		}
	}
	if(!end) msg_out = ""; // reset if we haven't found an end
	//cout << "msg_out: " << msg_out << endl;
	return msg_out;
}



// option, but may not be used so much
string encode_task(Item<int> task){
	Command command;
	return command.A + "=" + to_string(task.get());
}

// mainly use this, it's easier
string encode_task(Items<int> tasks){
	Command command;
	return command.A + "=" + to_string(tasks.get_last_item());
}



string encode_image_params(Items<int> tasks, bool obst_found, float obst_dist, int theta, int crossing){
	string msg = encode_task(tasks, msg); // first part of the message, send task
	
	// if what you were looking for is found in the image (line, ball...)
	msg += "," + command.OF + "=" + to_string(obst_found);

	if (obst_found){

		int task = tasks.get_last_item();

		if (task != ARUCO){
			// distance to the obstacle
			msg += "," + command.OD + "=" + to_string(theta);

			// heading error parameter -> int or float???
			msg += "," + command.TH_t + "=" + to_string(theta);

			if (task == LINE){
				// send which kind of crossing has been found
				msg += "," + command.CROSS + "=" + to_string(crossing);
			}
		}
		else{ // task == ARUCO
			// decide which data to share!!!
			msg += "";
		}
	}
	return msg;
	
}


void decode(string msg, Controllers & c, Robot_params r){
	// detect if the message is 
	string msg = detect_message(string msg);
	if (msg != ""){
		// split the message
	    vector<string> words = split_str(msg, "=,");    // utils
	    Command command;

	    // save it as new target pose
	    for (uint i=0; i<words.size(); i++){
	        if(words.at(i) == command.A){
	            int val = str2int(words.at(i+1));
	            if (val != BIG_INT) {
	                sens.set_s(val);
	                i++;
	            }
	        }
	        else if(words.at(i) == command.X_w){
	            float val = str2float(words.at(i+1));
	            if (val != BIG_FLOAT) {
	                sens.set_x(val);
	                i++;
	            }
	        }
    	}
	}
}