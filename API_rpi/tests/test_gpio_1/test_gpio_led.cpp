#include <iostream>
//#include <wiringPi.h>

#include "../../raspberry_pi/leds.h"


using namespace std;


void first_test(){
    cout << "Start GPIO test 1..." << endl;
    wiringPiSetup () ;
    int pin_test = 7;
    pinMode (pin_test, OUTPUT) ;
    for (;;)
    {
        digitalWrite (pin_test, HIGH) ; delay (500) ;
        digitalWrite (pin_test,  LOW) ; delay (500) ;
        cout << "Blink" << endl;
    }

    cout << "Stopping GPIO test 1..." << endl;
}

void test_leds_library(){
    Leds leds;
    while(true){
        
        cout << "\n  ka";
        leds.keep_alive();

        cout << "\n  leader";
        leds.is_leader(1);          //does nothing with this leds
       
        cout << "\n  task";
        leds.task_doing(1);
        
        cout << "\n  plan";
        leds.navigating(1);
        

        cout << "\n\n************";
        
    }

}

int main (void){
    
    //first_test();         //working
    test_leds_library();

    return 0;
}