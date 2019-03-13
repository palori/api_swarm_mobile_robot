#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;



class Subscriber
{
public:
	Subscriber(); 					// empty constructor, use the default port
	Subscriber(int port, string publisher_ip);
	~Subscriber();
	void connect();
	void disconnect();
	string listen();					// while true (listening)


	// Getters
	bool get_debug();
	int get_port();
	string get_topic();
	bool get_is_connected();
	string get_publisher_ip();


	// Setters
	void set_debug(bool b);
	void set_port(int i);
	void set_topic(string s);
	void set_is_connected(bool b);
	void set_publisher_ip(string s);


//private:
	void setup();
	void set_publish_endpoint();

	bool debug = true;
	const string ENDPOINT = "tcp://"; //always stays the same
	int port = 6000; // default port
	string topic = "";
	string publish_endpoint = ""; // needs update whenever the port is updated
	bool is_connected = false;
	string publisher_ip = "localhost";

	// Create a Subscriber socket
	zmqpp::context context;
	zmqpp::socket_type type = zmqpp::socket_type::subscribe;
	zmqpp::socket *sok = NULL;	// need to be pointer (has no empty constructor)

};
