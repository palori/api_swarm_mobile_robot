/*
OPTIONS:

http://www.webalice.it/fede.tft/serial_port/serial_port.html
https://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing
http://xanthium.in/Serial-Port-Programming-on-Linux
*/

#include "../comm_serial.h"

//using namespace std;
//using namespace std::chrono;


void test_read_serial(){
    int fd = serial_open();
    if (fd != NULL){
        time_t timer0, timer1, timer2;
        double s1, s2;

        time(&timer0);
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        read_char_by_char(fd, true); //good, 0s (resolution 1s)
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        time(&timer1);
        //read_char_array(fd); //buggs and not so fast
        time(&timer2);

        s1 = difftime(timer1,timer0);
        s2 = difftime(timer2,timer0);
        printf ("\n'read_char_by_char': %.f seconds.", s1);
        printf ("\n'read_char_array'  : %.f seconds.", s2);

        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "\n\n'read_char_by_char': " << time_span.count()*1000.0 << " milliseconds.\n";

        close(fd);
        printf("\n  ttyUSB0 Closed Successfully\n");
    }
}

void test_write_serial(){
    string msg = "@s=500,x=0.22,y=0.53,th=0.08$";

    int fd = serial_open();
    if (fd != NULL){
        serial_write(fd, msg, true);
    }
}

int main(){
    //test_read_serial();           //working
    test_write_serial();

    return 0;
}