#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../../raspberry_pi/leds.h"
using namespace std;

void sig_handler(int sig);

bool ctrl_c_pressed = false;

int main (void)
{

    struct sigaction sig_struct;
    sig_struct.sa_handler = sig_handler;
    sig_struct.sa_flags = 0;
    sigemptyset(&sig_struct.sa_mask);

    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
        cout << "Problem with sigaction" << endl;
        exit(1);
    }

    Leds leds;


    while(!ctrl_c_pressed)
    {
        leds.keep_alive();

    }


    cout << "Exiting....." << endl;
    return 0;
}

void sig_handler(int sig)
{
    write(0,"\nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}