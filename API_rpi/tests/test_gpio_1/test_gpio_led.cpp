#include <iostream>
#include <wiringPi.h>

//#include "../../raspberry_pi/leds.h"


using namespace std;


int main (void)
{
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
    return 0 ;
}