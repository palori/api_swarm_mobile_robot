#include "../utils.h"
#include <iostream>
#include <vector>
//#include "../comm_rpi_2.h"
#include "../item.h"
#include "../robot_params.h"
#include "../sensors.h"
#include "../controllers.h"

//#import "graph.h"

using namespace std;

void test_split_string(){
	
	vector<string> words;
	cout << "words out: " << words.size() << endl;
	words = split_str("x=1,y=3", "=,");
	cout << "words out: " << words.size() << endl;
	cout << words.size() << " words:" << endl;
	for (uint i=0; i<words.size(); i++){
		cout << words.at(i) << endl;
	}
	
}

/*
void test_msg2sensorData(){
	string msg = "@x=1,y=2$";
	Sensors sens;
	COMM_RPI cr;
	cr.msg2sensorData(msg, sens);
}
*/

/*
void test_add2vector_print_vector(){	//worked when it was in utils
	vector<string> v;
	int max_len = 2;
	v = add2vector(v, "holaw", max_len);
	print_vector(v);
	v = add2vector(v, "adeu", max_len);
	print_vector(v);
	v = add2vector(v, "fins aviat", max_len);
	print_vector(v);

	vector<int> v10;
	v10 = add2vector(v10, 4, max_len);
	print_vector(v10);
	v10 = add2vector(v10, 5, max_len);
	print_vector(v10);
	v10 = add2vector(v10, 6, max_len);
	print_vector(v10);

	vector<float> v1;
	v1 = add2vector(v1, 1.0, max_len);
	print_vector(v1);
	v1 = add2vector(v1, 2.0, max_len);
	print_vector(v1);
	v1 = add2vector(v1, 3.0, max_len);
	print_vector(v1);

	vector<bool> v2;
	v2 = add2vector(v2, true, max_len);
	print_vector(v2);
	v2 = add2vector(v2, false, max_len);
	print_vector(v2);
	v2 = add2vector(v2, true, max_len);
	print_vector(v2);
	
}
*/

void test_item_template(){	// working

	cout << endl << "Test 'Item' template" << endl;
	Item<string> it("hi");
	cout << "item: " << it.get() << endl;

	Item<int> it1(2);
	cout << "item: " << it1.get() << endl;

	Item<float> it2(3.4);
	cout << "item: " << it2.get() << endl;

}


void test_items_template(){ // working

	cout << endl << "Test 'Items' template" << endl;
	
	Items<string> its("string_ex",2);

	its.print_items();
	its.add_item("hola");
	its.print_items();
	
	Item<string> it("hi");
	its.add_item(it.get());
	its.print_items();

	Item<string> it1("bye");
	its.add_item(it1.get_noMutex());
	its.print_items();

	cout << endl << "last item: " << its.get_last_item() << endl;

}

void test_robot_params(){		// working
	Robot_params rob("potato", 6000, 2);
	cout << "hi!" << endl;
	rob.x.add_item(2.0);
	rob.print_info();
}


void test_sensors(){
	Sensors sens(2);
	cout << "hi!" << endl;
	//cout << sens.s.get_MAX_LEN() << endl;
	//cout << sens.s.get_last_item_noMutex() << endl;
	sens.x.add_item(1.0);
	sens.x.add_item(2.0);
	sens.obst_found.add_item(true);
	sens.print_info();
	sens.x.print_items();
}

void test_controllers(){
	Controllers ctrl;
	cout << "hi!" << endl;
	//cout << ctrl.s.get_MAX_LEN() << endl;
	//cout << ctrl.s.get_last_item_noMutex() << endl;
	ctrl.x_t.add_item(12.5);
	ctrl.x_t.add_item(24.75);
	ctrl.stop.add_item(true);
	ctrl.print_info();
	ctrl.x_t.print_items();
}

//  compile from terminal!!!!!!!!!!!
//  g++ -O -W -Wall -pedantic -o test_utils -p ../utils.cpp ../comm_rpi_2.cpp test_utils.cpp ../sensors.cpp
//  or make (now working!)

int main(int argc, char const *argv[])
{
	//test_split_string();
	//test_msg2sensorData();
	//test_add2vector_print_vector();
	//test_item_template();
	//test_items_template();
	//test_robot_params();
	test_sensors();
	test_controllers();

	return 0;
}

