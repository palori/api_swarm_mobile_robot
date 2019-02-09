/*
	demo_serial_msg.cpp
*/

//#include "../comm/comm_msg.h"
//#include "../comm/comm_serial.h"
#include <thread>
#include "../comm_rpi.h"


/****************
 * SERIAL TESTS *
 ****************/

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







/*************
 * MSG TESTS *
 *************/

void test_msg2target(){
    string msg = "00000000000,@,s,x=0.22,y=0.53,th=0.08,$,0000";
    target new_pose;
    new_pose.servo = 200.0;
    new_pose.y = 11.11;
    print_target(new_pose);
    msg2target(new_pose, msg, true);
    msg = "00000000000,@,sx=10,y1,th=1.75,$,0000";
    msg2target(new_pose, msg, true);
    msg = "00000000000,,s=300,x=10,y=2,th=1.75,$,0000";
    msg2target(new_pose, msg, true);
    msg = "00000000000,@,x=10,y=3.3";
    msg2target(new_pose, msg, true);
}

void test_target2msg(){
    string msg = "";
    target new_pose;
    cout << "0. message= " << msg << endl;

    new_pose.servo = 1.0;
    new_pose.x = 2.0;
    new_pose.y = 3.0;
    new_pose.th = 4.0;
    target2msg(new_pose, msg, true, false);
    cout << "1. message= " << msg << endl;

    new_pose.servo = 11.0;
    new_pose.x = 22.0;
    new_pose.y = 33.0;
    new_pose.th = 44.0;
    target2msg(new_pose, msg, true, false);
    cout << "2. message= " << msg << endl;
}





/****************
 * FULL EXAMPLE *
 ****************/

void demo_real_example()
{	

    /*
    1. listen to serial
    2. decode message
    3. compute new target
    4. encode target
    5. send to serial
    */

	// initialization
	string msg = "";
	target pose, target_pose;
	pose.servo = 0.0;
    pose.x = 0.0;
    pose.y = 0.0;
    pose.th = 0.0;
    //old_pose = pose;
    target_pose = pose;
    target_pose.x = 0.5;
    int fd = 0, freq = 0;
    bool start = true;
    bool connected2teensy = false;


    while(!connected2teensy){
        fd = serial_open(false);
        if (fd > 1){

            msg = "@,rpi_conn,$";
            serial_write(fd, msg, true);

            msg = serial_read(fd, true); // 1
            //serial_close(fd, false);
            msg2connect(msg);
            if (msg=="tsy_conn")
            {
                connected2teensy = true;
                printf("Connected\n");
            }
            usleep(1000000);
        }
    }
    serial_close(fd, false);


	while(true){

        // read serial and update target
        fd = serial_open(false);
        if (fd > 1){
            msg = serial_read(fd, true); // 1
            //serial_close(fd, false);
        	msg2target(pose, msg, false); // 2    
            usleep(1000);
        }

        freq += 1;
        if (freq>=50 || start)
        {
            freq = 0;
            printf("\nCicle\n");
            if(target_pose.x - pose.x < 0.2 || start){
                if (start) start = false;

                printf("\nSend new target\n");
                // compute and send new target
                target_pose.x = pose.x + 0.5; // 3
            }    
                if (target_pose.x > 100.0) pose.x = 0.0;
                target2msg(target_pose, msg, false, false); // 4

                // write data to serial
                fd = serial_open(false);
                if(fd > 1){
                    serial_write(fd, msg, true); // 5
                    usleep(1000);
                    serial_close(fd, false);
                }
            //}
        }

        
            
    }
}




/***********************************************
 * MAIN - SELECT THE FUNCTION YOU WANT TO TEST *
 ***********************************************/

int main(){

    /* SERIAL TESTS */
    //test_read_serial();                                           //working
    //test_write_serial("@,s=0.0,x=77.77,y=0.50,th=0.0,$");         //working
    //test_loop_read_write();                                         //working
    
    /* MSG TESTS */
    //test_msg2target();              //working
    //test_target2msg();              //working

    /* FULL EXAMPLE */
    demo_real_example();              //working -> good one ready to use

    return 0;
}