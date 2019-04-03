#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;



class Publisher
{
public:
	Publisher(); 					// empty constructor, use the default port
	Publisher(int port);
	~Publisher();
	void connect();
	void disconnect();
	void publish(string msg);


	// Getters
	bool get_debug();
	int get_port();
	string get_topic();
	bool get_is_connected();

	// Setters
	void set_debug(bool b);
	void set_port(int i);
	void set_topic(string s);
	void set_is_connected(bool b);


	Publisher & operator=(Publisher & pub);


//private:
	void setup();
	void set_publish_endpoint();

	bool debug = true;
	const string ENDPOINT = "tcp://*:"; //always stays the same
	int port = 6000; // default port
	string topic = "";
	string publish_endpoint = ""; // needs update whenever the port is updated
	bool is_connected = false;

	// Create a publisher socket
	zmqpp::context context;
	zmqpp::socket_type type = zmqpp::socket_type::publish;
	zmqpp::socket *sok = NULL;	// need to be pointer (has no empty constructor)

};
