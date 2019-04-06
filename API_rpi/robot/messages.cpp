#include "messages.h"



string detect_message(string msg){
	//string msg = "00@a=1,b=1,l=3.00$99";
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
			store = false;
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




// mainly use this, it's easier
string encode_task(int task){
	Command command;
	return "@" + command.A + "=" + to_string(task) + "$";
}



string encode_image_params(int task, bool obst_found, float obst_dist, float theta, int crossing){
	Command command;

	string msg = "@" + command.A + "=" + to_string(task);
	
	// if what you were looking for is found in the image (line, ball...)
	msg += "," + command.OF_i + "=" + to_string(obst_found);

	if (obst_found){

		if (task != ARUCO){
			// distance to the obstacle
			msg += "," + command.OD_i + "=" + to_string(obst_dist);

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
	msg += "$";
	return msg;
	
}


/*
 * params2msg
 * 
 * // encode the new params to send the message
 * USING string -> WORKING IN RPi
 */
/*void params2msg(string & msg){        // STILL TO WORK ON IT!

	// @@@@ 'send_only_if_updaded' this might mean that we need to keep track of the previous pose
	// NOT implemented for the moment

	string new_msg = "@";
	new_msg += "s=" + to_string(0.0);
	new_msg += ",x=" + to_string(0.0);
	new_msg += ",y=" + to_string(0.0);
	new_msg += ",th=" + to_string(0.0);
	new_msg += "$";

	// update message
	msg = new_msg;

	if (get_debug()) cout << "Message: " << msg << endl;
}  */






void decode_task(string msg, Items<int> & tasks){
	// detect if the message is 
	msg = detect_message(msg);
	if (msg != ""){
		// split the message
		vector<string> words = split_str(msg, "=,");    // utils
		Command command;

		// save it as new target pose
		for (uint i=0; i<words.size(); i++){
			if(words.at(i) == command.A){
				int val = str2int(words.at(i+1));
				if (val != BIG_INT) {
					tasks.add_item(val);
					i++;
				}
			}
		}
	}
}




// NOT fully tested, but the important ones yes
void decode_ctrl(string msg, Controllers & ctrl){
	// detect if the message is 
	msg = detect_message(msg);
	if (msg != ""){
		// split the message
		vector<string> words = split_str(msg, "=,");    // utils
		Command command;

		// save it as new target pose
		for (uint i=0; i<words.size(); i++){
			if(words.at(i) == command.A){
				int val = str2int(words.at(i+1));
				if (val != BIG_INT) {
					ctrl.tasks.add_item(val);
					//rob.tasks.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.B){
				int i_val = str2int(words.at(i+1));
				if (i_val != BIG_INT) {
					

					if (i_val>-1 && i_val<2){
						switch(ctrl.tasks.get_last_item()){
							//case CONNECT: ctrl.connect.add_item(i_val); break;
							case RESET_ENC: ctrl.reset_enc.add_item(i_val); break;
							case STOP: ctrl.stop.add_item(i_val); break;
							case AVOID_OBSTACLES: ctrl.avoid_obst.add_item(i_val); break;

							case IR_ON:
								ctrl.ir_on.add_item(i_val);
								if (i_val==0 && ctrl.ir_send.get_last_item()) ctrl.ir_send.add_item(0); // if stop then also stop sending
								break;
							case IR_SEND:
								if (i_val==1) ctrl.ir_on.add_item(i_val);
								ctrl.ir_send.add_item(i_val);
								break;
							case IMU_ON:
								ctrl.imu_on.add_item(i_val);
								if (i_val==0){ // if stop then also stop sending
									ctrl.imu_gyro_send.add_item(0);
									ctrl.imu_acc_send.add_item(0);
									ctrl.imu_comp_send.add_item(0);
								}
								break;
							case IMU_GYRO_SEND:
								if (i_val==1) ctrl.imu_on.add_item(i_val);
								ctrl.imu_gyro_send.add_item(i_val);
								break;
							case IMU_ACC_SEND:
								if (i_val==1) ctrl.imu_on.add_item(i_val);
								ctrl.imu_acc_send.add_item(i_val);
								break;
							case IMU_COMP_SEND:
								if (i_val==1) ctrl.imu_on.add_item(i_val);
								ctrl.imu_comp_send.add_item(i_val);
								break;

							case MOTORS_ON: ctrl.motors_on.add_item(i_val); break;

							//case DEBUG: set_debug(i_val); break;

							//default: if(get_debug()){cout << "Incorrect action." << endl;} break;
								
						}
					}
					i++;
				}
			}
			
			else if(words.at(i) == command.FWD){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.fwd_dist.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.TRN){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.trn_deg.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.TRNR){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.trn_r.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.V){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.vel.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.S){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.s.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.OD){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.obst_dist.add_item(val);
					i++;
				}
			}



			else if(words.at(i) == command.X_t){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.x_t.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.Y_t){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.y_t.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.TH_t){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					ctrl.th_t.add_item(val);
					i++;
				}
			}



			else if(words.at(i) == command.KP){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					switch (ctrl.tasks.get_last_item()){
						case SET_PID_M1: ctrl.m1_kp.add_item(val); break;
						case SET_PID_M2: ctrl.m2_kp.add_item(val); break;
						case SET_PID_TH: ctrl.th_kp.add_item(val); break;
						//default: if(get_debug()){Serial.println("Set PID not enabled.");}
					}
					i++;
				}
			}
			else if(words.at(i) == command.KI){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					switch (ctrl.tasks.get_last_item()){
						case SET_PID_M1: ctrl.m1_ki.add_item(val); break;
						case SET_PID_M2: ctrl.m2_ki.add_item(val); break;
						case SET_PID_TH: ctrl.th_ki.add_item(val); break;
						//default: if(get_debug()){Serial.println("Set PID not enabled.");}
					}
					i++;
				}
			}

		}
	}
}










void decode_robot_params(string msg, Robot_params & rob){
	// detect if the message is 
	msg = detect_message(msg);
	if (msg != ""){
		// split the message
		vector<string> words = split_str(msg, "=,");    // utils
		Command command;

		// save it as new target pose
		for (uint i=0; i<words.size(); i++){
			if(words.at(i) == command.A){
				int val = str2int(words.at(i+1));
				if (val != BIG_INT) {
					rob.tasks.add_item(val);
					i++;
				}
			}
			

			else if(words.at(i) == command.X_w){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					rob.x.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.Y_w){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					rob.y.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.TH_w){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					rob.th.add_item(val);
					i++;
				}
			}

		}
	}
}






void decode_image(string msg, Sensors & sens, string & new_target){
	// detect if the message is 
	msg = detect_message(msg);
	if (msg != ""){
		// split the message
		vector<string> words = split_str(msg, "=,");    // utils
		Command command;

		// save it as new target pose
		for (uint i=0; i<words.size(); i++){

			/*
			if(words.at(i) == command.A){				// do we need this?
				int val = str2int(words.at(i+1));
				if (val != BIG_INT) {
					sens.tasks.add_item(val);
					i++;
				}
			}*/
			

			if(words.at(i) == command.OF_i){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.obst_found_img.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.CROSS){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.cross.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.TH_t){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.th_t.add_item(val);
					new_target = "@" + command.TH_t + "=" + words.at(i+1) + "$"; 		// just send this to TSY, the rest for localization
					i++;
				}
			}
			else if(words.at(i) == command.OD_i){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.obst_dist_img.add_item(val);
					i++;
				}
			}

		}
	}
}








/*
 * comes from COMM_RPI
 */
void decode_sensors(string msg, Sensors & sens){

	msg = detect_message(msg);
	if (msg != ""){
		// split the message
		vector<string> words = split_str(msg, "=,");    // utils
		Command command;

		// save it as new target pose
		for (uint i=0; i<words.size(); i++){
			//cout << " for [" << i << "] = " << words.at(i) << endl;
			if(words.at(i) == command.S){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.s.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.X_w){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.x.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.Y_w){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.y.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.TH_w){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.th.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.IR1){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.ir1.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.IR2){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.ir2.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.OD){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.obst_dist.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.OF){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.obst_found.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.GYRO1){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.gyro_x.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.GYRO2){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.gyro_y.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.GYRO3){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.gyro_z.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.ACC1){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.acc_x.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.ACC2){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.acc_y.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.ACC3){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.acc_z.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.COMP1){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.comp_x.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.COMP2){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.comp_y.add_item(val);
					i++;
				}
			}
			else if(words.at(i) == command.COMP3){
				float val = str2float(words.at(i+1));
				if (val != BIG_FLOAT) {
					sens.comp_z.add_item(val);
					i++;
				}
			}
		}
	}

}