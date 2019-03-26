#include "messages.h"

void encode_task(Tasks t, string & msg){
	Command command;
	msg = command.A + "=" + to_string(t.get_task());
}

void encode_params(Tasks t, string & msg){
	
}


void decode(string msg, Controllers & c){


}