
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
void COMM_TSY::write_serial(String msg){ //params might change, maybe add last sensors data
    // modify to add this
    //sensorData2msg(msg);






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

    set_action(-1); // reset ??????????????????????????????
	String msg = Serial.readString();
    Serial.println("MSG: "+ msg);
    msg2params(msg);
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
void COMM_TSY::msg2params(String msg){

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
        if (get_debug()) Serial.println("pch="+String(pch));

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

                    case IR_ON: set_ir_on(i_val); break;
                    case IR_SEND: set_ir_send(i_val); break;
                    case IMU_ON: set_imu_on(i_val); break;
                    case IMU_GYRO_SEND: set_imu_gyro_send(i_val); break;
                    case IMU_ACC_SEND: set_imu_acc_send(i_val); break;
                    case IMU_COMP_SEND: set_imu_comp_send(i_val); break;

                    case MOTORS_ON: set_motors_on(i_val); break;

                    case DEBUG: set_debug(i_val); break;

                    default: if(get_debug()){Serial.println("Incorect action.");} break;
                        
                }
            
            }
        } 


        // The following commands can be sent at any time
        else if (words[i] == command.FWD){
            val = words[i+1].toFloat();
            if (val != BIG_INT) {set_fwd_dist(val);}

        }
        else if (words[i] == command.TRN){
            val = words[i+1].toFloat();
            if (val != BIG_INT) {set_trn_deg(val);}

        }
        else if (words[i] == command.TRNR){
            val = words[i+1].toFloat();
            if (val != BIG_INT) {set_trn_r(val);}

        }
        else if (words[i] == command.V){
            val = words[i+1].toFloat();
            if (val != BIG_INT) {set_vel(val);}
        }
        else if (words[i] == command.S){
            val = words[i+1].toFloat();
            if (val != BIG_INT) {set_servo(val);}

        }
        else if (words[i] == command.OD){
            // obstacle distance -> IR will force to stop if distance
            // is <=od and avoid_obst==true
            val = words[i+1].toFloat();
            if (val != BIG_INT) {set_obst_dist(val);}
        }



        // The following commands need to follow the action of setting a PID
        else if (words[i] == command.KP){
            val = words[i+1].toFloat();
            if (val != BIG_INT) {
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
            if (val != BIG_INT) {
                switch (get_action()){
                    case SET_PID_M1: set_m1_ki(val); break;
                    case SET_PID_M2: set_m2_ki(val); break;
                    case SET_PID_TH: set_th_ki(val); break;
                    default: if(get_debug()){Serial.println("Set PID not enabled.");}
                }
            }
        }
        else {
            if(get_debug()){Serial.println("Incorect parameter.");}
            case_found = false;
        }

        if (case_found) i++; // jump the already used value and inspect the next param

    } // for loop

    delete[] str; // deallocate dynamic memory
    delete[] words;
}


// encode the mesage --> old version, need to be updated
void COMM_TSY_TSY::sensorData2msg(String & msg){

	// @@@@ 'send_only_if_updaded' this might mean that we need to keep track of the previous pose
	// NOT implemented for the moment

	String new_msg = "@";
    /*if (CPU_IS_RASPBERRY){
        new_msg += ",s=" + to_string(new_pose.servo);
        new_msg += ",x=" + to_string(new_pose.x);
        new_msg += ",y=" + to_string(new_pose.y);
        new_msg += ",th=" + to_string(new_pose.th);
    }
    else{*/
    	new_msg += ",s=" ;//+ String(new_pose.servo);
    	new_msg += ",x=" ;//+ String(new_pose.x);
    	new_msg += ",y=" ;//+ String(new_pose.y);
    	new_msg += ",th=" ;//+ String(new_pose.th);
    //}
    new_msg += ",$";
	// update message
	msg = new_msg;

	if (get_debug()) Serial.println("Message: " + msg);
}	



