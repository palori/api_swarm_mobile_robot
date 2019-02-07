
#include "comm_serial.h"

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




