
#include <iostream>
#include <vector>
#include "../../leader_election/bully.h"


using namespace std;

void test_trigger_election(){
	Bully bully(3);
	vector<int> robots_ids;
	for (int i = 409; i <= 1000; i++){
		robots_ids.push_back(i);
	}
	bully.trigger_election(robots_ids);
}

int	main(int argc, char const *argv[]){
	// tests
	test_trigger_election();
	return 0;
}