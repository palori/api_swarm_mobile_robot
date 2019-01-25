
#ifndef COMM_SERIAL_H
#define COMM_SERIAL_H

#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <iostream>
#include <string.h>
#include <vector>
#include <time.h>
#include <unistd.h> // time sleep microseconds
#include <chrono> // the best one to count time in [s, ms, ns...]
//#include <Arduino.h>
using namespace std;
using namespace std::chrono;

int serial_open(bool print_msg);
void serial_close(int fd, bool print_msg);


void serial_write(int fd, string msg, bool print_msg);

string serial_read(int fd, bool print_msg);


#endif