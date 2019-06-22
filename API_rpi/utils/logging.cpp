
#include "logging.h"


Logging::Logging(){}

Logging::~Logging(){}

Logging::Logging(string filename, string extension){
	init(filename, extension);	
}

void Logging::init(string file_name, string ext){
	this->filename.set(file_name);
	this->extension.set(ext);
	string full = file_name + "_" + get_start_time() + "." + ext;
	this->full_filename.set(full);
}

void Logging::log(string line){

	string new_line = get_time_now() + "," + line + "\n";
	std::ofstream log(full_filename.get(), std::ios_base::app | std::ios_base::out);
	log << new_line;

	cout << "add line: " << new_line;
}


string Logging::get_time_now(){
	auto now = chrono::system_clock::now();
	auto ms = chrono::duration_cast< chrono::milliseconds >(now.time_since_epoch());
	long l = ms.count(); // by default the type is long (but just in case)
	string s = xtos(l);
	//printf("  Milliseconds:\t%lu\n", (l));
	//cout << "  Milliseconds:\t" << s << "\n";
	return s;
}


string Logging::get_start_time(){
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer,80,"%F_%T",timeinfo);
	puts (buffer);
	string s = string(buffer);
	cout << "start time: " << s << endl;
	return s;
}