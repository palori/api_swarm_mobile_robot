
#include <iostream>

#include "../../utils/logging.h"


using namespace std;

void test(){
	Logging logg;
	logg.init("../../logs/hola", "csv");

	logg.log("Bon dia!");
	logg.log("Bona nit :)");
}

int	main(int argc, char const *argv[]){
	// tests
	test();
	return 0;
}