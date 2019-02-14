
#include "comm_1.h"


COMM::COMM(){

}

COMM::~COMM(){

}



//uint OUT_BUFFER_SIZE = 32;
//char c_msg[OUT_BUFFER_SIZE];

/*void init_output_buffer(){
    for (uint i = 0; i < OUT_BUFFER_SIZE; ++i){
        c_msg[i] = '0';
    }
}*/
void COMM::write_serial(String msg){ //params might change

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


void COMM::read_serial(){
	return Serial.readString();
}


/*
	FROM HERE COPIED FROM 'comm_msg.h'
*/


// have to go with struct target
//void COMM::print_target(target new_pose){

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
void COMM::msg2params(target & new_pose, String msg, bool print_msg){

    // split the message
    int len = msg.length();
    char * str = new char[len];
    for(int i=0; i< len; i++){
    	str[i] = msg[i];
    }
    char * pch;
    //vector<String> words;
    int count=0;

    /*if (CPU_IS_RASPBERRY){
        string * words = new string[len];
    }
    else{*/
        String * words = new String[len];
    //}
    bool keep_reading=true, store=false;
    pch = strtok (str," ,="); //" ,.-"
    while (pch != NULL && keep_reading)
    {
        //Serial.println("pch="+String(pch)); // for debugging
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
    bool case_found;
    for (int i=0; i<count; i++){ // no need to go through all array, just the words saved
    	float val = BIG_FLOAT;
        int i_val = BIG_INT;

        case_found = true;
        switch(words[i]){
            case "a": // action
                i_val = words[i+1].toInt();
                if (i_val != BIG_INT) {
                    set_action(i_val);
                }
                break;

            case "b": // value of the boolean actions
                i_val = words[i+1].toInt();
                if (i_val>-1 && i_val<2) { // should only be 0 or 1
                    switch(get_action()){
                        case CONNECT: set_connect(i_val); break;
                        case RESET_ENC: set_reset_enc(i_val); break;
                        case STOP: set_stop(i_val); break;
                        case AVOID_OBSTACLES: set_avoid_obst(i_val); break;

                        case IR_ON: set_ir_on(i_val); break;
                        case IR_SEND: set_ir_send(i_val); break;
                        case IMU_ON: set_imu_on(i_val); break;
                        case IMU_GYRO_SEND: set_imu_gyro_send(i_val); break;
                        case IMU_ACC_SEND: set_imu_acc_send(i_val); break;
                        case IMU_COMP_SEND: set_imu_comp_send(i_val); break;

                        case MOTORS_ON: set_motors_on(i_val); break;

                        default: if(get_debug()){Serial.println("incorect action");} break;
                            
                    }
                
                }
                break;

            case "":
                //
                break;

            case "":
                //
                break;

            case "":
                //
                break;

            default:
                case_found = false;
        }
        if (case_found) i++;




        if(words[i] == "s"){
            //val = words.at(i+1).toFloat();
            val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.servo = val;
                i++;
            }
        }
        else if(words[i] == "x"){
            //val = words.at(i+1).toFloat();
            val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.x = val;
                i++;
            }
        }
        else if(words[i] == "y"){
            //val = words.at(i+1).toFloat();
            val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.y = val;
                i++;
            }
        }
        else if(words[i] == "th"){
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

    delete[] str; // deallocate dynamic memory
    delete[] words;
}


// encode the target to send the message
void sensors2msg(target new_pose, String & msg, bool print_msg, bool send_only_if_updaded){

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
    	new_msg += ",s=" + String(new_pose.servo);
    	new_msg += ",x=" + String(new_pose.x);
    	new_msg += ",y=" + String(new_pose.y);
    	new_msg += ",th=" + String(new_pose.th);
    //}
    new_msg += ",$";
	// update message
	msg = new_msg;

	if (print_msg){
        //if (CPU_IS_RASPBERRY) printf("Message: " + msg);
        //else 
            Serial.println("Message: " + msg);
    }
}	



