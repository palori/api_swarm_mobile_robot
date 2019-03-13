#include "p.h"



#if INHERITANCE
// option 1: inheritance
p::p(zmqpp::context context, zmqpp::socket_type type):
	zmqpp::socket(context, type){
	/*
	zmqpp::context context;
	zmqpp::socket_type type = zmqpp::socket_type::publish;
	zmqpp::socket(context, type);
	*/
	
	//socket(context, type);

	bind(PUBLISH_ENDPOINT);
	cout << "Binding to " << PUBLISH_ENDPOINT << "..." << endl;
	this_thread::sleep_for(chrono::milliseconds(1000));
}

/*p::p(int i){
	cout << "P constructor " << i << "..." << endl;
	p();
}*/

p::~p(){}


void p::do_something(){
	
	string text = "inheritance...";

	// Create a message and feed data into it
	zmqpp::message message;
	message << text;
	send(message);

	cout << "[SENT]: " << text << endl;
}



#else
// option 2: pointer to the socket
p::p(){
	//zmqpp::context context;
	//zmqpp::socket_type type = zmqpp::socket_type::publish;
	sok = new zmqpp::socket(context, type);

	sok->bind(PUBLISH_ENDPOINT);
	cout << "  Binding to " << PUBLISH_ENDPOINT << "..." << endl;
	this_thread::sleep_for(chrono::milliseconds(1000));
	do_something();
	cout << "  end constructor" << endl;
}

p::p(int i){
	cout << "'p' constructor " << i << "..." << endl;
	p();
	cout << "end constructor " << i << "..." << endl;
}

p::~p(){
	delete(sok);
}


void p::do_something(){
	cout << "    in 'do_something()'..." << endl;


	string text = "pointer...";

	// Create a message and feed data into it
	zmqpp::message message;
	message << text;

	sok->send(message);

	cout << "    [SENT]: " << text << endl;
}
#endif





//////////
// TEST //
//////////

int main(int argc, char *argv[]) {


#if INHERITANCE
	cout << "Start test" << endl;

	zmqpp::context context;
	zmqpp::socket_type type = zmqpp::socket_type::publish;

	p pub(context, type);
	cout << 20 << endl; // never reaches this line, it gets stuck before
	pub.do_something();
	cout << "End test" << endl;
#else
	cout << "Start test" << endl;
	p pub;//(4);
	cout << "Now do 'do_something()'" << endl; // never reaches this line, it gets stuck before
	pub.do_something();
	cout << "End test" << endl;
#endif
}