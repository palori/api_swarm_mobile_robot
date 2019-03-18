#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <chrono>
#include <thread>

//#define INHERITANCE 1

using namespace std;

static const string PUBLISH_ENDPOINT = "tcp://*:4242";



#if INHERITANCE
// option 1: inheritance
class p: public  zmqpp::socket//:socket(context, type)
{
public:
	p(zmqpp::context context, zmqpp::socket_type type);//:zmqpp::socket(context, type);
	//p(int i);
	~p();

	void do_something();
	
};



#else
// option 2: pointer to the socket
class p //: public  thread
{
public:
	zmqpp::context context;
	zmqpp::socket_type type = zmqpp::socket_type::publish;
	zmqpp::socket *sok = NULL;


	p();
	p(int i);
	~p();

	void do_something();
	
};
#endif

