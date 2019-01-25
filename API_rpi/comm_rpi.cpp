
#include "comm_rpi.h"

int serial_open(bool print_msg){

    // Just in case some possible port was open, let's close them
    // I'm not sure how good that is. Maybe other programs are using the others...
    /*int num_serial_ports = 6; // maybe less
    for (int i = 2; i < num_serial_ports; ++i)
    {
        close(i);
    }*/
    // I've seen that usually the port is #3, let's close just this one just in case (for my Ubuntu)
    //close(3);

    int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY); // the serial port might be different in the raspi
    if (print_msg){
        if(fd==1){
            printf("\n  Error! in Opening ttyUSB0\n");
        }
        else
            printf("\n  ttyUSB0 Opened Successfully\n");
    }
    return fd;
}

void serial_close(int fd, bool print_msg){
    close(fd);
    if (print_msg) printf("\n  ttyUSB0 Closed Successfully\n");
}


void serial_write(int fd, string msg, bool print_msg){

    vector<char> c_msg(msg.c_str(), msg.c_str() + msg.size() + 1); //str2vector_char
    char ccc[c_msg.size()];
    for(uint i=0; i < c_msg.size(); i++){
        ccc[i] = c_msg[i];
    }
    write(fd, ccc, c_msg.size());           // @@@@ improvement, it returns some value that could be used to know if wrote or not
    /*for(int i=0; i < c_msg.size(); i++){
        write(fd, c_msg[i], 1);
    }*/

    if(print_msg){
        cout << "message sent: " << msg << endl;
    }
}

string serial_read(int fd, bool print_msg){ //read_char_by_char
    //char *c = new char[1];
    char c[1]={'1'};
    bool keep_reading=true, store=false;
    string msg="";

    const int MAX_CHAR2READ = 100;
    int char_read = 0;
    try{
        while(keep_reading){
            //cout << "hola!\n";
            int cs = read(fd,&c,sizeof(c));  // @@@@ need some timeout if not reading anything!!! -> maybe adding a queue
            if(cs>0){
                //cout << c[0];
                
                if(c[0]=='@' && !store){
                    store=true;
                }
                else if(c[0]=='$' && store){
                    keep_reading=false;
                    //store = false;
                    //cout << endl;
                }
                if(store) {
                    msg+=c[0];
                }
            }
            char_read+=1;
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
    if (print_msg) cout << "msg: " << msg << endl;

    return msg;
}



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




/*
// decode the received message into target
void msg2target(target & new_pose, string msg, bool print_msg){

    // split the message
    int len = msg.length();
    char * str = new char[len];
    for(int i=0; i< len; i++){
        str[i] = msg[i];
    }
    char * pch;
    //vector<String> words;
    int count=0;

    //if (CPU_IS_RASPBERRY){
        string * words = new string[len];
    /*}
    else{
        String * words = new String[len];
    }*/
/*
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
    const float BIG_NUM = 191919.191919;
    for (int i=0; i<count; i++){ // no need to go through all array, just the words saved
        float val = BIG_NUM;
        if(words[i] == "s"){
            val = str2float(words[i+1]);
            //val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.servo = val;
                i++;
            }
        }
        else if(words[i] == "x"){
            val = str2float(words[i+1]);
            //val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.x = val;
                i++;
            }
        }
        else if(words[i] == "y"){
            val = str2float(words[i+1]);
            //val = words[i+1].toFloat();
            if (val != BIG_NUM) {
                new_pose.y = val;
                i++;
            }
        }
        else if(words[i] == "th"){
            val = str2float(words[i+1]);
            //val = words[i+1].toFloat();
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
}*/

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
