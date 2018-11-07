/*
OPTIONS:

http://www.webalice.it/fede.tft/serial_port/serial_port.html
https://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing
http://xanthium.in/Serial-Port-Programming-on-Linux
*/


#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <iostream>
//#include <string>
using namespace std;

int serial_open(){
    int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    if(fd==1)
        printf("\n  Error! in Opening ttyUSB0\n");
    else
        printf("\n  ttyUSB0 Opened Successfully\n");
    return fd;
}

/*
void serial_write(int fd, char c[]){
    for(int i=0; i < sizeof(c); i++){
        write(fd, c[i], 1);
    }
}*/

void read_char_by_char(int fd){
    //char *c = new char[1];
    char c[1]={'1'};
    bool keep_reading=true, store=false;
    string msg="";

    try{
        while(keep_reading){
            int cs = read(fd,&c,sizeof(c));
            if(cs>0){
                cout << c[0];

                if(c[0]=='@'){
                    store=true;
                }else if(c[0]=='$' && store){
                    keep_reading=false;
                    cout << endl;
                }
                if(store){
                    msg+=c[0];
                }
            }
        }
    }
    catch (exception& e)
    {
        cout << e.what() << '\n';
        cout << "msg: " << msg << endl;
    }
    cout << "msg: " << msg << endl;
}

void read_char_array(int fd){
    int len = 64;
    char c[len];

    while(c[0]!='$'){
        int cs = read(fd,&c,len);
        if(cs>0){
            cout << "read: " << c << endl;
        }
    }
}


int main(){
    int fd = serial_open();

    read_char_by_char(fd);
    //read_char_array(fd);
    
    //write(fd, "22\r", 3);

    close(fd);
    printf("\n  ttyUSB0 Closed Successfully\n");
    return 0;
}



