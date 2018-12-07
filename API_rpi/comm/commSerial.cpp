
#include "comm_serial.h"

int serial_open(){
    int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    if(fd==1){
        printf("\n  Error! in Opening ttyUSB0\n");
    	return fd;
    }
    else
        printf("\n  ttyUSB0 Opened Successfully\n");
    return NULL;
}


void serial_write(int fd, string msg, bool print_msg){

    vector<char> c_msg(msg.c_str(), msg.c_str() + msg.size() + 1); //str2vector_char
    char ccc[c_msg.size()];
    for(unsigned int i=0; i < c_msg.size(); i++){
        ccc[i] = c_msg[i];
    }
    write(fd, ccc, c_msg.size());
    /*for(int i=0; i < c_msg.size(); i++){
        write(fd, c_msg[i], 1);
    }*/

    if(print_msg){
        cout << "message sent: " << msg << endl;
    }
}

string read_char_by_char(int fd, bool print_msg){
    //char *c = new char[1];
    char c[1]={'1'};
    bool keep_reading=true, store=false;
    string msg="";

    try{
        while(keep_reading){
            int cs = read(fd,&c,sizeof(c));
            if(cs>0){
                //cout << c[0];
                if(c[0]=='@' && !store){
                    store=true;
                }
                else if(c[0]=='$' && store){
                    keep_reading=false;
                    store = false;
                    //cout << endl;
                }
                else if(c[0]!='$' && store) {
                    msg+=c[0];
                }
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

//// can be deleted
//// more complicated and there is a bug (and may take more time?)
/*
void read_char_array(int fd){
    int l = 100;
    char c[l];
    for (int i=0; i<l; i++){
        c[i] = '0';
    }
    bool keep_reading=true, store=false;
    string msg="";
    try{
        while(keep_reading){
            int cs = read(fd,&c,l);
            if(cs>0){
                cout << "read: " << c << endl;
                for (int i=0; i<l; i++){
                    if(c[i]=='@' && !store){
                        store=true;
                    }
                    else if(c[i]=='$' && store){
                        keep_reading=false;
                        store = false;
                        break; // leave for loop
                        //cout << endl;
                    }
                    else if(c[i]!='$' && store) {
                        msg+=c[i];
                    }
                }
            }
        }
    }
    catch (exception& e)
    {
        cout << e.what() << '\n';
    }
    cout << "msg: " << msg << endl;
}
*/


