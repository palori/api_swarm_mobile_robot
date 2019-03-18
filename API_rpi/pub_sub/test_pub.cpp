#include "../publisher.h"


int main(int argc, char *argv[]) {

	Publisher pub(4000);
	//pub.disconnect();
	//pub.set_port(500);
	//pub.connect();
	pub.publish("Can I publish now???");
}