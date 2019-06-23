
#ifndef logging_h
#define logging_h

#include <ios>
#include <iostream>
#include <fstream>

#include <string>
#include <string.h>

#include <stdio.h>      /* printf */
#include <chrono>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */


#include "utils.h"
#include "item.h"



using namespace std;

class Logging
{
public:
	Logging();
	~Logging();
	Logging(string filename, string extension);

	Item<string> filename;
	Item<string> extension;
	Item<string> full_filename;

	bool first_row;

	// start time to add to the file name

	void init(string file_name, string ext);

	void log(string line); // add time step every log


	string get_time_now();

	string get_start_time();
	
};

#endif