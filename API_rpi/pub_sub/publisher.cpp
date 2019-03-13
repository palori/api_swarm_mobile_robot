#include "publisher.h"

////////////////////
// Public methods //
////////////////////

Publisher::Publisher(){
	// automatically connects and opens to the port
	setup();
}


Publisher::Publisher(int port){
	set_port(port);
	setup();
}

void Publisher::setup(){
	sok = new zmqpp::socket(context, type);
	set_is_connected(false); 	// default = false, but just in case
	connect();
}


Publisher::~Publisher(){
	delete(sok);
}


void Publisher::connect(){
	if (!get_is_connected()){
		set_is_connected(true);
		// Open the connection
		set_publish_endpoint();
		if (get_debug()) cout << "Binding to " << publish_endpoint << "..." << endl;
		sok->bind(publish_endpoint);

		// Pause to connect
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
	else{
		cout << "Already connected, 'disconnect()' and 'connect()' again if the port was changed..." << endl;
	}
}


void Publisher::disconnect(){
	// Might never be used but good practice to have it
	if (get_is_connected()){
		set_is_connected(false);
		sok->disconnect(publish_endpoint);
	}
	else{
		cout << "Not connected yet, use 'connect()'..." << endl;
	}

}


void Publisher::publish(string msg){
	if (get_is_connected()){
		// Create a message and feed data into it
		zmqpp::message message;
		message << msg;

		// publish
		sok->send(message);

		if (get_debug()) cout << "    [SENT]: " << msg << endl;
	}
	else{
		cout << "Not connected yet, use 'connect()'..." << endl;
	}
}


// Getters
bool Publisher::get_debug(){return debug;}
int Publisher::get_port(){return port;}
string Publisher::get_topic(){return topic;}
bool Publisher::get_is_connected(){return is_connected;}


// Setters
void Publisher::set_debug(bool b){debug = b;}
void Publisher::set_port(int i){port = i;}
void Publisher::set_topic(string s){topic = s;}
void Publisher::set_is_connected(bool b){is_connected = b;}




/////////////////////
// Private methods //
/////////////////////
void Publisher::set_publish_endpoint(){ 
	publish_endpoint = ENDPOINT + to_string(get_port());
}











//////////
// TEST //
//////////

int main(int argc, char *argv[]) {

	Publisher pub(4000);
	//pub.disconnect();
	//pub.set_port(500);
	//pub.connect();
	pub.publish("Can I publish now???");
}