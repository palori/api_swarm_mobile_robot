
#include "comm_rpi_2.h"

COMM_RPI::COMM_RPI(){}
COMM_RPI::~COMM_RPI(){}

void COMM_RPI::serial_open(){

    // Just in case some possible port was open, let's close them
    // I'm not sure how good that is. Maybe other programs are using the others...
    /*int num_serial_ports = 6; // maybe less
    for (int i = 2; i < num_serial_ports; ++i)
    {
        close(i);
    }*/
    // I've seen that usually the port is #3, let's close just this one just in case (for my Ubuntu)
    //close(3);

    int fd = open(USB_SERIAL_PORT, O_RDWR | O_NOCTTY);
    set_fd(fd);

    if(get_fd()==1) set_port_open(false);
    else set_port_open(true);


    if (get_debug()){
        if(get_fd()==1) printf("\n  Serial port: error opening\n");
        else printf("\n  Serial port: opened\n");
    }
}


void COMM_RPI::serial_close(){
    if(get_port_open()){
        close(get_fd());
        set_fd(-1);         //might not be needed (setting it in 'serial_open')
        set_port_open(false);
        if (get_debug()) printf("\n  Serial port: closed\n");
    }
    else {if(get_debug()) printf("\n  Serial port: no port to close\n");}
}


void COMM_RPI::serial_write(string msg){
    /* old options
    vector<char> c_msg(msg.c_str(), msg.c_str() + msg.size() + 1); //str2vector_char
    char ccc[c_msg.size()];
    for(uint i=0; i < c_msg.size(); i++){
        ccc[i] = c_msg[i];
    }

    write(fd, ccc, c_msg.size()); */          // @@@@ improvement, it returns some value that could be used to know if wrote or not
    /*for(int i=0; i < c_msg.size(); i++){
        write(fd, c_msg[i], 1);
    }*/


    //if(!get_port_open()) serial_open();       // might want to use this
    if(get_port_open()){
        write(get_fd(), msg.c_str(), msg.size());
        if(get_debug()) cout << "Serial port: message sent \"" << msg << "\"" << endl;
    }
    else {if(get_debug()) cout << "Serial port: can't write, port closed" << endl;}
}

string COMM_RPI::serial_read(){ //read_char_by_char

    if(get_port_open()){
        //char *c = new char[1];
        char c[1]={'1'};
        bool keep_reading=true, store=false;
        
        bool use_string = true;

            // with strings -> recommended not to use them
            string msg="";
            // with c_strings
            const int MAX_CHAR2READ = 100;
            //char * msg = new char[MAX_CHAR2READ]; @@@@ can be done with static strings
            //char msg[MAX_CHAR2READ]; // uncomment to use c_string
            int msg_count = 0;
            //end with, msg[msg_count] = '\0' or 0;
        
        
        
        int char_read = 0;
        try{
            while(keep_reading){
                //cout << "hola!\n";
                int cs = read(fd,&c,sizeof(c));  // @@@@ need some timeout if not reading anything!!! -> maybe adding a queue
                if(cs>0){
                    //cout << c[0];
                    
                    if(c[0]==START && !store){
                        store=true;
                    }
                    else if(c[0]==END && store){
                        keep_reading=false;
                        //store = false;
                        //cout << endl;
                    }
                    if(store) {
                        if (use_string){
                            msg+=c[0];
                        }
                        else{
                            msg[msg_count]+=c[0];
                            msg_count++;
                        }
                        
                    }
                }
                else{
                    break; // read nothing
                }
                char_read+=1; // @@@@ sleep the thread (1-2-10 ms)
                //cout << char_read;
                if (char_read>=MAX_CHAR2READ){
                    break;
                }

            }
        }
        catch (exception& e)
        {
            cout << e.what() << '\n';
            //cout << "msg: " << msg << endl;
        }

        if (!use_string){
            msg[msg_count]+='\0';
        }

        if (get_debug()) cout << "msg: " << msg << endl;

        return msg;
    }
    else {if(get_debug()) cout << "Serial port: can't read, port closed" << endl;}
    return "";
}




/*
 * msg2target
 *
 * decode the received message
 * USING VECTOR -> WORKING!
 */
void COMM_RPI::msg2sensorData(string msg, Sensors & sens){          // STILL TO WORK ON IT!

    // split the message
    vector<string> words = split_str(msg, "=,");    // utils
    Command command;

    // save it as new target pose
    for (uint i=0; i<words.size(); i++){
        if(words.at(i) == command.S){
            float val = str2float(words.at(i+1));
            if (val != BIG_FLOAT) {
                //new_pose.servo = val;
                sens.set_s(val);
                i++;
            }
        }
        else if(words.at(i) == command.X_w){
            float val = str2float(words.at(i+1));
            if (val != BIG_FLOAT) {
                //new_pose.x = val;
                sens.set_x(val);
                i++;
            }
        }
        else if(words.at(i) == command.Y_w){
            float val = str2float(words.at(i+1));
            if (val != BIG_FLOAT) {
                //new_pose.y = val;
                sens.set_y(val);
                i++;
            }
        }
        else if(words.at(i) == command.TH_w){
            float val = str2float(words.at(i+1));
            if (val != BIG_FLOAT) {
                //new_pose.th = val;
                sens.set_th(val);
                i++;
            }
        }
        else if(words.at(i) == command.IR1){
            float val = str2float(words.at(i+1));
            if (val != BIG_FLOAT) {
                //new_pose.th = val;
                sens.set_th(val);
                i++;
            }
        }

        // ...
        // missing others (ir1, ir2, obst_dist, obst_found, gyro[3], acc[3], comp[3])
    }

    if (get_debug()){ // show that results are saved
        //print_target(new_pose);
        cout << "target: " << endl;
    }
}




/*
 * params2msg
 * 
 * // encode the new params to send the message
 * USING string -> WORKING IN RPi
 */
void COMM_RPI::params2msg(string & msg){        // STILL TO WORK ON IT!

    // @@@@ 'send_only_if_updaded' this might mean that we need to keep track of the previous pose
    // NOT implemented for the moment

    string new_msg = START;
    new_msg += "s=" + to_string(0.0);
    new_msg += ",x=" + to_string(0.0);
    new_msg += ",y=" + to_string(0.0);
    new_msg += ",th=" + to_string(0.0);
    new_msg += END;

    // update message
    msg = new_msg;

    if (get_debug()) cout << "Message: " << msg << endl;
}   





