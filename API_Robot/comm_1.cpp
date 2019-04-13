
#include "comm_1.h"


COMM_TSY::COMM_TSY(){

}

COMM_TSY::~COMM_TSY(){
	/* Do I need to do it here or in the function, I guess
	 * in the function because they are local variables
	 * (not attributes)
	delete[] str; // deallocate dynamic memory
	delete[] words;
	 */
}



//uint OUT_BUFFER_SIZE = 32; // JCA suggestion
//char c_msg[OUT_BUFFER_SIZE];

/*void init_output_buffer(){
	for (uint i = 0; i < OUT_BUFFER_SIZE; ++i){
		c_msg[i] = '0';
	}
}*/
void COMM_TSY::write_serial(double _odo[3], float _ir[2], float battery ,int _imu_cmps[3], int _imu_gyro[3], int _imu_accel[3], bool _obstacle_found){ //params might change, maybe add last sensors data
	// modify to add this
	String msg = sensorData2msg(_odo, _ir, battery, _imu_cmps, _imu_gyro, _imu_accel, _obstacle_found);


	// String 2 char[]
	int len = msg.length();
	//String len_s = String(len);
	//Serial.println("len_s: "+len_s);
	char * c_msg = new char[len];
	/*for (int i = 0; i < len; ++i){
		c_msg[i] = '0';
	}
	Serial.write(c_msg);*/
	for (int i = 0; i < len; ++i){
		c_msg[i] = msg[i];
	}
	//Serial.println(c_msg);
	Serial.write(c_msg);

	delete[] c_msg; // deallocate dynamic memory
}


void COMM_TSY::read_serial(){

	if (get_debug()) Serial.println("connect: "+String(get_connect()));
	set_action(-1); // reset ??????????????????????????????

	/* OLD WAY
	String msg = Serial.readString();
	if (get_debug()) Serial.println("MSG: "+ msg);
	msg2params(msg);
	*/

	// NEW WAY
	if (buf_count < BUF_LEN) {
		if (Serial.available()) {
			char c = Serial.read();
			if (c=='@') {
				buf_count = 0;            // always begin anew when '@' received
				buf[buf_count] = '\0';    // reset the buffer
				new_msg = true;
			}
			else if (new_msg && c=='$'){
				buf[buf_count] = '\0';
				new_msg = false;
				buf_length = String(buf).length();

				//Serial.println("buf: "+String(buf));

				msg2params();

			}
			else if (new_msg){
				buf[buf_count] = c;
				buf_count++;
			}
			
			if (buf_count > BUF_LEN){ // check always
				buf_count = 0;
				new_msg = false;
			}
		}
	}
	if (get_debug()) Serial.println("buf: "+String(buf)+", new: "+String(new_msg));
	
}



String COMM_TSY::to_string(){
	
	String deb = "  Action: " + String(get_action());
	deb += "\n  Connect: " + String(get_connect());
	deb += "\n  Reset encoders: " + String(get_reset_enc());
	deb += "\n  Stop: " + String(get_stop());
	deb += "\n  Avoid obstacles: " + String(get_avoid_obst());
	deb += "\n  Obstacle distance to detect: " + String(get_obst_dist());
	deb += "\n";
	deb += "\n  IR on: " + String(get_ir_on());
	deb += "\n  IR send: " + String(get_ir_send());
	deb += "\n  IMU on: " + String(get_imu_on());
	deb += "\n  IMU gyro send: " + String(get_imu_gyro_send());
	deb += "\n  IMU acc send: " + String(get_imu_acc_send());
	deb += "\n  IMU comp send: " + String(get_imu_comp_send());
	deb += "\n";
	deb += "\n  Motors on: " + String(get_motors_on());
	deb += "\n  Max speed: " + String(get_vel());
	deb += "\n";
	deb += "\n  M1 Kp: " + String(get_m1_kp());
	deb += "\n  M1 Ki: " + String(get_m1_ki());
	deb += "\n  M2 Kp: " + String(get_m2_kp());
	deb += "\n  M2 Ki: " + String(get_m2_ki());
	deb += "\n  Th Kp: " + String(get_th_kp());
	deb += "\n  Th Ki: " + String(get_th_ki());
	deb += "\n";
	deb += "\n  Forward distance: " + String(get_fwd_dist());
	deb += "\n  Turn angle: " + String(get_trn_deg());
	deb += "\n  Turn radius: " + String(get_trn_r());
	deb += "\n  Servo position: " + String(get_servo());

	return deb;
}

void COMM_TSY::debug_params(){
	String deb = "\n\n*********\n* Debug *\n*********\n";
	deb += to_string() + "\n*********\n\n";

	if (get_debug()){
		Serial.print(deb);
	}
}



/*
	FROM HERE COPIED FROM 'comm_msg.h'
*/


// have to go with struct target
//void COMM_TSY::print_target(target new_pose){

	/*if (CPU_IS_RASPBERRY){
		printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nTarget:");
		printf("\n    Servo = " + to_string(new_pose.servo));
		printf("\n    x = " + to_string(new_pose.x));
		printf("\n    y = " + to_string(new_pose.y));
		printf("\n    th = " + to_string(new_pose.th));
		printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	}
	else {*/
/*        Serial.print("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nTarget:");
		Serial.print("\n    Servo = "); Serial.print(new_pose.servo);
		Serial.print("\n    x = "); Serial.print(new_pose.x);
		Serial.print("\n    y = "); Serial.print(new_pose.y);
		Serial.print("\n    th = "); Serial.print(new_pose.th);
		Serial.print("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	//}

}*/



// decode the received message into target
// Called within 'read_serial()' !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void COMM_TSY::msg2params(){// OLD WAY: (String msg){
	/* OLD WAY
	// split the message
	int len = msg.length();
	char * str = new char[len];
	for(int i=0; i< len; i++){
		str[i] = msg[i];
	}
	char * pch;
	int count=0; // number of items in the array 'words'
	String * words = new String[len]; // will store the read parametes and their values

	bool keep_reading=true, store=false;
	pch = strtok (str," ,=");

	while (pch != NULL && keep_reading)
	{
		if (get_debug()) Serial.pr intln("pch="+String(pch));

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
		}
		pch = strtok (NULL, " ,=");
	}
	*/

	// NEW WAY
	char * str = new char[buf_length];
	for(int i=0; i< buf_length; i++){
		str[i] = buf[i];
	}

	char * pch;
	int count=0; // number of items in the array 'words'
	String * words = new String[String(str).length()]; // will store the read parametes and their values

	bool keep_reading=true;
	pch = strtok (str," ,=");

	while (pch != NULL && keep_reading)
	{
		if (get_debug()) Serial.println("pch="+String(pch));

		words[count] = pch;
		count++;
		pch = strtok (NULL, " ,=");
	}





	// save the read params
	bool case_found;
	Command command;

	if (get_debug()){
		Serial.println("  msg2params: "+String(count/2)+" params");
		Serial.println("count = *"+String(count)+"*");
	}

	for (int i=0; i<count; i++){ // no need to go through all array, just the words saved
		float val = BIG_FLOAT;
		int i_val = BIG_INT;
		case_found = true;

		if (get_debug()) Serial.println("    words["+String(i)+"]="+words[i]);

		if (words[i] == command.A){
		//case command.A: // action
			if (get_debug()) Serial.println("  a");
			i_val = words[i+1].toInt();
			if (i_val != BIG_INT) {
				set_action(i_val);
			}
		}
		else if (words[i] == command.B){// value of the boolean actions
			i_val = words[i+1].toInt();
			if (get_debug()){
				Serial.println("  b=");
				Serial.println("    words["+String(i+1)+"]="+words[i+1]);
				Serial.println("    i_val="+String(i_val));
			}
			if (i_val>-1 && i_val<2) { // should only be 0 or 1
				switch(get_action()){
					case CONNECT: set_connect(i_val); break;
					case RESET_ENC: set_reset_enc(i_val); break;
					case STOP: set_stop(i_val); Serial.println("    Stop = :"+String(i_val));break;
					case AVOID_OBSTACLES: set_avoid_obst(i_val); break;

					case IR_ON:
						set_ir_on(i_val);
						if (i_val==0 && get_ir_send()) set_ir_send(0); // if stop then also stop sending
						break;
					case IR_SEND:
						if (i_val==1) set_ir_on(i_val);
						set_ir_send(i_val);
						break;
					case IMU_ON:
						set_imu_on(i_val);
						if (i_val==0){ // if stop then also stop sending
							if (get_imu_gyro_send()) set_imu_gyro_send(0);
							if (get_imu_acc_send()) set_imu_acc_send(0);
							if (get_imu_comp_send()) set_imu_comp_send(0);
						}
						break;
					case IMU_GYRO_SEND:
						if (i_val==1) set_imu_on(i_val);
						set_imu_gyro_send(i_val);
						break;
					case IMU_ACC_SEND:
						if (i_val==1) set_imu_on(i_val);
						set_imu_acc_send(i_val);
						break;
					case IMU_COMP_SEND:
						if (i_val==1) set_imu_on(i_val);
						set_imu_comp_send(i_val);
						break;

					case MOTORS_ON: set_motors_on(i_val); break;

					case DEBUG: set_debug(i_val); break;

					case FWD: set_fwd(i_val); break;
					case TRN: set_trn(i_val); break;
					case TRNR: set_trnr(i_val); break;
					case DRIVE: set_drive(i_val); break;
					case FOLLOW: set_followline(i_val); break;
					case RACE: set_race(i_val); break;
					case STAIRS: set_stairs(i_val); break;

					default: if(get_debug()){Serial.println("Incorrect action.");} break;
						
				}
			
			}
		} 


		// The following commands can be sent at any time
		else if (words[i] == command.FWD){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_fwd_dist(val);}

		}
		else if (words[i] == command.TRN){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_trn_deg(val);}

		}
		else if (words[i] == command.TRNR){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_trn_r(val);}

		}
		else if (words[i] == command.V){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_vel(val);}
		}
		else if (words[i] == command.S){
			val = words[i+1].toInt();
			if (val != BIG_FLOAT) {
				if (val) set_servo(SERVO_ON);
				else set_servo(SERVO_OFF);
			}

		}
		else if (words[i] == command.OD){
			// obstacle distance -> IR will force to stop if distance
			// is <=od and avoid_obst==true
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_obst_dist(val);}
		}

		else if (words[i] == command.X_t){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_x_t(val);}
		}
		else if (words[i] == command.Y_t){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_y_t(val);}
		}
		else if (words[i] == command.TH_t){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_th_t(val);}
		}

		else if (words[i] == command.X_0){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_x_0(val);}
		}
		else if (words[i] == command.Y_0){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_y_0(val);}
		}
		else if (words[i] == command.TH_0){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {set_th_0(val);}
		}

		// The following commands need to follow the action of setting a PID
		else if (words[i] == command.KP){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {
				switch (get_action()){
					case SET_PID_M1: set_m1_kp(val); break;
					case SET_PID_M2: set_m2_kp(val); break;
					case SET_PID_TH: set_th_kp(val); break;
					default: if(get_debug()){Serial.println("Set PID not enabled.");}
				}
			}
		}
		else if (words[i] == command.KI){
			val = words[i+1].toFloat();
			if (val != BIG_FLOAT) {
				switch (get_action()){
					case SET_PID_M1: set_m1_ki(val); break;
					case SET_PID_M2: set_m2_ki(val); break;
					case SET_PID_TH: set_th_ki(val); break;
					default: if(get_debug()){Serial.println("Set PID not enabled.");}
				}
			}
		}
		else {
			if(get_debug()){Serial.println("Incorrect parameter.");}
			case_found = false;
		}

		if (case_found) i++; // jump the already used value and inspect the next param

	} // for loop

	// OLD WAY: delete[] str; // deallocate dynamic memory
	delete[] words;
}


// encode the mesage --> old version, need to be updated
String COMM_TSY::sensorData2msg(double _odo[3], float _ir[2], float battery,int _imu_cmps[3], int _imu_gyro[3], int _imu_accel[3], bool _obstacle_found){
	Command command;

	/*
		if most of them want to be send, might want to split them
		into two messages (or more)
	*/



	String new_msg = "@";

	new_msg += command.X_w+"=" + String(_odo[0]);
	new_msg += ","+command.Y_w+"=" + String(_odo[1]);
	new_msg += ","+command.TH_w+"=" + String(_odo[2]);

	if (get_ir_send()){
		new_msg += ","+command.IR1+"=" + String(_ir[0]);
		new_msg += ","+command.IR2+"=" + String(_ir[1]);
	}

	new_msg += ","+command.BATT + "=" +String(battery);

	if (get_avoid_obst()){
		new_msg += ","+command.OD+"=" + String(get_obst_dist());
		new_msg += ","+command.OF+"=" + String(_obstacle_found);
	}

	if (get_imu_gyro_send()){
		new_msg += ","+command.GYRO1+"=" + String(_imu_gyro[0]);
		new_msg += ","+command.GYRO2+"=" + String(_imu_gyro[1]);
		new_msg += ","+command.GYRO3+"=" + String(_imu_gyro[2]);
	}

	if (get_imu_acc_send()){
		new_msg += ","+command.ACC1+"=" + String(_imu_accel[0]);
		new_msg += ","+command.ACC2+"=" + String(_imu_accel[1]);
		new_msg += ","+command.ACC3+"=" + String(_imu_accel[2]);
	}

	if (get_imu_comp_send()){
		new_msg += ","+command.COMP1+"=" + String(_imu_cmps[0]);
		new_msg += ","+command.COMP2+"=" + String(_imu_cmps[1]);
		new_msg += ","+command.COMP3+"=" + String(_imu_cmps[2]);
	}

	new_msg += "$";

	if (get_debug()) Serial.println("Message: " + new_msg);
	return new_msg;
}	


