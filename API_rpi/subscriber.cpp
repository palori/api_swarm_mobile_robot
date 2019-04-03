#include "subscriber.h"

////////////////////
// Public methods //
////////////////////

Subscriber::Subscriber(){
	// automatically connects and opens to the port
	setup();
}


Subscriber::Subscriber(int port, string publisher_ip){
	set_port(port);
	set_publisher_ip(publisher_ip);
	setup();
}

void Subscriber::setup(){
	sok = new zmqpp::socket(context, type);
	sok->subscribe(get_topic());	// Subscribe to a channel
	set_is_connected(false); 		// default = false, but just in case
	connect();
}


Subscriber::~Subscriber(){
	delete(sok);
}


void Subscriber::connect(){
	if (!get_is_connected()){
		set_is_connected(true);
		// Open the connection
		set_publish_endpoint();
		if (get_debug()) cout << "Connecting to " << publish_endpoint << "..." << endl;
		sok->connect(publish_endpoint);
	}
	else{
		cout << "Already connected, 'disconnect()' and 'connect()' again if the port was changed..." << endl;
	}
}


void Subscriber::disconnect(){
	// Might never be used but good practice to have it
	if (get_is_connected()){
		set_is_connected(false);
		sok->disconnect(publish_endpoint);
	}
	else{
		cout << "Not connected yet, use 'connect()'..." << endl;
	}

}


string Subscriber::listen(){
	string msg = "";
	if (get_is_connected()){
		// Receive (blocking call)
		zmqpp::message message;
		sok->receive(message);

		// Read as a string
		message >> msg;

		if (get_debug()) cout << "    [SENT]: " << msg << endl;
	}
	else{
		cout << "Not connected yet, use 'connect()'..." << endl;
	}
	return msg;
}


// Getters
bool Subscriber::get_debug(){return debug;}
int Subscriber::get_port(){return port;}
string Subscriber::get_topic(){return topic;}
bool Subscriber::get_is_connected(){return is_connected;}
string Subscriber::get_publisher_ip(){return publisher_ip;}


// Setters
void Subscriber::set_debug(bool b){debug = b;}
void Subscriber::set_port(int i){port = i;}
void Subscriber::set_topic(string s){topic = s;}
void Subscriber::set_is_connected(bool b){is_connected = b;}
void Subscriber::set_publisher_ip(string s){publisher_ip = s;}


Subscriber & Subscriber::operator=(Subscriber & subs){
	set_debug(subs.get_debug());
	set_port(subs.get_port());
	set_topic(subs.get_topic());
	set_is_connected(subs.get_is_connected());
	set_publisher_ip(subs.get_publisher_ip());
	return *this;
}



/////////////////////
// Private methods //
/////////////////////
void Subscriber::set_publish_endpoint(){ 
	publish_endpoint = ENDPOINT + get_publisher_ip() + ":" + to_string(get_port());
}



