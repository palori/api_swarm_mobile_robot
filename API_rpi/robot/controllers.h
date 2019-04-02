#ifndef controllers_h
#define controllers_h

#include "utils.h"
#include "item.h"

using namespace std;


class Controllers
{
public:
	Controllers();
	~Controllers();


	// Attributes
	int MAX_LEN = 10;		// default

	Items<int> tasks;		// do we need this??

	//Items<float> connect;
	Items<bool> reset_enc;
	Items<bool> stop;
	Items<bool> avoid_obst;
	Items<float> obst_dist;

	Items<bool> ir_on;
	Items<bool> ir_send;

	Items<bool> imu_on;
	Items<bool> imu_gyro_send;
	Items<bool> imu_acc_send;
	Items<bool> imu_comp_send;		

	Items<bool> motors_on;
	Items<float> vel;

	Items<float> m1_kp;
	Items<float> m1_ki;
	Items<float> m2_kp;
	Items<float> m2_ki;
	Items<float> th_kp;
	Items<float> th_ki;

	Items<float> fwd_dist;
	Items<float> trn_deg;
	Items<float> trn_r;
	Items<float> s;					// might not be used at the end...
	
	Items<float> x_t;
	Items<float> y_t;
	Items<float> th_t;


	int get_MAX_LEN();
	void set_MAX_LEN(int i);

	void print_info();

	Controllers & operator=(Controllers & ctrl);
	
private:
	void init_items();
	
};

#endif