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


int main(){
    int fd = serial_open();

    //char *c = new char[1];
    char c[1]={'1'};

    while(c[0]!='0'){
        int cs = read(fd,&c,sizeof(c));
        if(cs>0){
            cout << "read: " << c[0] << endl;
        }
    }
    write(fd, "22\r", 3);

    close(fd);
    printf("\n  ttyUSB0 Closed Successfully\n");
    return 0;
}



