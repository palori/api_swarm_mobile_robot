/*
OPTIONS:

http://www.webalice.it/fede.tft/serial_port/serial_port.html
https://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing
http://xanthium.in/Serial-Port-Programming-on-Linux
*/

#include "../comm_serial.h"
#include <thread>

//using namespace std;
//using namespace std::chrono;


string test_read_serial(){
    string msg = "";
    int fd = serial_open(false);
    if (fd != NULL){
        time_t timer0, timer1, timer2;
        double s1, s2;

        time(&timer0);
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        msg = serial_read(fd, true); //good, 0s (resolution 1s)
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        time(&timer1);
        //read_char_array(fd); //buggs and not so fast
        //time(&timer2);

        s1 = difftime(timer1,timer0);
        //s2 = difftime(timer2,timer0);
        printf ("\n'serial_read': %.f seconds.", s1);
        //printf ("\n'read_char_array'  : %.f seconds.", s2);

        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "\n\n'serial_read': " << time_span.count()*1000.0 << " milliseconds.\n";

        serial_close(fd, false);
    }
    return msg;
}

void test_write_serial(string msg){
    //string msg = "@,s=500,x=0.22,y=0.53,th=0.08,$";

    int fd = serial_open(false);
    if (fd != NULL){
        serial_write(fd, msg, true);
    }
    serial_close(fd, false);
}

void test_loop_read_write(){
    string msg = "";
    while(true){
        printf("\n\n  READ\n");
        msg = test_read_serial();           //working
        std::this_thread::sleep_for (seconds(1)); // delete after tests
        printf("\n\n  Write\n");
        test_write_serial(msg);             //working
        std::this_thread::sleep_for (seconds(2)); // delete after tests
    }
}

int main(){
    //test_read_serial();                                               //working
    //test_write_serial("@,s=0.0,x=77.77,y=0.50,th=0.0,$");             //working
    
    test_loop_read_write();

    return 0;
}