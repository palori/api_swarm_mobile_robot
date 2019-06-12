#include "../../comm/pub_subs/subscriber.h"

int main(int argc, char *argv[]) {

	Subscriber subs(4000, "localhost");
	cout << "listening..." << endl;
	subs.listen();
}