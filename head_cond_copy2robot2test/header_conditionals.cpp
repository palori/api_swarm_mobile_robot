
#include "header_conditionals.h"
#include <iostream>
#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <string.h>

void func1(){}

void func2(){}

void func3(){printf("Hola\n");}

#ifdef CPU_IS_RASPBERRYPI
	char * func4(char * msg){
		//cout << endl << "holaaa" << endl;
		printf("\n\n\nprinting %s!!!\n\n\n", msg);
		//printf("In Raspberry the message is: %i\n", msg);
		msg[0] = 'I';
		return msg;
	}
#else // CPU_IS_ARDUINO
	void func4(String msg){
		Serial.print("In Arduino the message is: ");
		Serial.println(msg);
	}
#endif

