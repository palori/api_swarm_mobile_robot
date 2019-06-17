
#include "leds.h"



Leds::Leds(){
	leader = new GPIOClass(xtos(LED_LEADER));
	ka = new GPIOClass(xtos(LED_TEST));				// change to LED_KA
	task = new GPIOClass(xtos(LED_TASK));
	plan_nav = new GPIOClass(xtos(LED_PLAN_NAV));
	cout << " GPIO pins created" << endl;

	leader->export_gpio();
	ka->export_gpio();
	task->export_gpio();
	plan_nav->export_gpio();
	cout << " GPIO pins exported" << endl;

    leader->setdir_gpio("out");
    ka->setdir_gpio("out");
    task->setdir_gpio("out");
    plan_nav->setdir_gpio("out");
    cout << " Set GPIO pin directions" << endl;
}

Leds::~Leds(){
	cout << "unexporting pins" << endl;
	leader->unexport_gpio();
	ka->unexport_gpio();
	task->unexport_gpio();
	plan_nav->unexport_gpio();

	cout << "deallocating GPIO Objects" << endl;
    delete leader;
    leader = 0;
    delete ka;
    ka = 0;
    delete task;
    task = 0;
    delete plan_nav;
    plan_nav = 0;
}

//void Leds::init();


// leader election (LE)
void Leds::is_leader(){

}

void Leds::election(){

}

// keep alive (KA) - to know that the robot is still on
void Leds::keep_alive(){
	ka->setval_gpio("1");
	usleep(1000000);
	ka->setval_gpio("0");
	usleep(1000000);
}

// task related
void Leds::task_doing(){}
void Leds::task_allocation(){}		// the leader computes it

// planning and navigation
void Leds::planning_route(){}
void Leds::navigating(){}

