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
#include <vector>
#include <time.h>
#include <unistd.h> // time sleep microseconds
#include <chrono> // the best one to count time in [s, ms, ns...]
//#include <Arduino.h>
using namespace std;
using namespace std::chrono;

int serial_open(){
    int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    if(fd==1)
        printf("\n  Error! in Opening ttyUSB0\n");
    else
        printf("\n  ttyUSB0 Opened Successfully\n");
    return fd;
}


void serial_write(int fd, string msg, bool print_msg){

    vector<char> c_msg(msg.c_str(), msg.c_str() + msg.size() + 1); //str2vector_char


    for(int i=0; i < c_msg.size(); i++){
        write(fd, c_msg[i], 1);
    }

    if(print){
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

void test_read_serial(){
    int fd = serial_open();

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

void test_write_serial(){
    string msg = "@s=500,x=0.22,y=0.53,th=0.08$"

    int fd = serial_open();
    serial_write(int fd, msg, true);
}

struct target{
    float x;
    float y;
    float th;
    float servo; // maybe not here?
}

float str2float(str s){
    try{
        return stof(s); 
    }
    catch(int e){
        cout << "Exception error nº " << e << endl;
        return 191919.1919;
    }
}

target msg2target(string msg){

    // split the message
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
            printf("store=true\n");
        }
        else if(*pch=='$' && store){
            keep_reading=false;
            store = false;
        }
        else if(*pch!='$' && store) {
            words.push_back(pch);
            printf ("%s\n",pch);
        }
        pch = strtok (NULL, " ,="); //" ,.-"
    }

    // save it as new target pose
    target new_pose;
    for (int i=0; i<words.size(); i++){
        if(words[i] == "s"){
            float val = str2float(words[i]);
            if (val != 191919.1919) {
                new_pose.servo = val;
                i++;
            }
        }
        else if(words[i] == "x"){
            float val = str2float(words[i]);
            if (val != 191919.1919) {
                new_pose.x = val;
                i++;
            }
        }
        else if(words[i] == "y"){
            float val = str2float(words[i]);
            if (val != 191919.1919) {
                new_pose.y = val;
                i++;
            }
        }
        else if(words[i] == "th"){
            float val = str2float(words[i]);
            if (val != 191919.1919) {
                new_pose.th = val;
                i++;
            }
        }
    }


    // show that results are saved
    cout << "\n\nNew target:\n    Servo = " << new_pose.servo;
    cout << "\n    x = " << new_pose.x;
    cout << "\n    y = " << new_pose.y;
    cout << "\n    th = " << new_pose.th << endl;

    return new_pose;
}


void other_tests(){

}


int main(){
    string msg = "00000000000,@,s=500,x=0.22,y=0.53,th=0.08,$,0000";
    target new_pose = msg2target(msg);
    //test_read_serial(); //working
    /*
    int fd = serial_open();
    while(true){
        try{
            //cout << "**** read **********************\n";
            //read_char_by_char(fd, true); //good, 0s (resolution 1s) -> need more precision

            cout << "**** write **********************\n";
            char ms[] = "\ts=500, x=0.22, y=0.53, th=0.08\n";
            cout << ms << endl << "\twith size of " << sizeof(ms) << endl;
            fd = serial_open();
            write(fd, ms, sizeof(ms));
            //write(fd, "22\r", 3);
            close(fd);
            usleep(2000000); //microseconds

        }
        catch (int e){
            printf("Exception %i occurred", e);
            break;
        }
    }
    close(fd);
    printf("\n  ttyUSB0 Closed Successfully\n");
    */
    return 0;
}



