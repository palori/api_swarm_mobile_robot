#ifndef comm_1_h
#define comm_1_h


#include "Arduino.h"
//#include "pins.h"
#include <string.h> // I think we only use String lib from Arduino
#include <errno.h>  /* ERROR Number Definitions */ //Might not be used any more

using namespace std;

/*
 * IF YOU MAKE ANY CHANGE IN THIS FILE, YOU HAVE TO MODIFY ACCORDINGLY
 * THE FOLLOWING FILES:
 * 
 *   - comm_1.h
 *   - comm_1.cpp
 *   - ../API_rpi/comm_rpi_1.h
 *   - ../API_rpi/comm_rpi_1.cpp
 */

class COMM_TSY
{
public:
	COMM_TSY();
	~COMM_TSY();
	void write_serial(double _odo[3], float _ir[2], int _imu_cmps[3], int _imu_gyro[3], int _imu_accel[3], bool _obstacle_found); // migth have diferent input params
	void read_serial();

	// getters if params is not a struct
	int get_action() {return action;}
	bool get_connect() {return connect;}
	bool get_reset_enc() {return reset_enc;} //not yet
	bool get_stop() {return stop;}
	bool get_avoid_obst() {return avoid_obst;}
	float get_obst_dist() {return obst_dist;}

	bool get_ir_on() {return ir_on;}
	bool get_ir_send() {return ir_send;}
	bool get_imu_on() {return imu_on;}
	bool get_imu_gyro_send() {return imu_gyro_send;}
	bool get_imu_acc_send() {return imu_acc_send;}
	bool get_imu_comp_send() {return imu_comp_send;}

	bool get_motors_on() {return motors_on;}
	float get_vel() {return vel;}

	float get_m1_kp() {return m1_kp;}
	float get_m1_ki() {return m1_ki;}
	float get_m2_kp() {return m2_kp;}
	float get_m2_ki() {return m2_ki;}
	float get_th_kp() {return th_kp;}
	float get_th_ki() {return th_ki;}

	float get_fwd_dist() {return fwd_dist;}
	float get_trn_deg() {return trn_deg;}
	float get_trn_r() {return trn_r;}
	float get_servo() {return servo;}

	bool get_debug() {return debug;}

	bool get_fwd() {return fwd;}
	bool get_trn() {return trn;}
	bool get_trnr() {return trnr;}
	bool get_drive() {return drive;}	// drive to target position
	bool get_followline() {return followline;}

	float get_x_t() {return x_t;}		// target position in Robot coord. syst.
	float get_y_t() {return y_t;}
	float get_th_t() {return th_t;}


	// setters if params is not a struct
	void set_action(int i) {action = i;}
	void set_connect(bool b) {connect = b;}
	void set_reset_enc(bool b) {reset_enc = b;}
	void set_stop(bool b) {stop = b;}
	void set_avoid_obst(bool b) {avoid_obst = b;}
	void set_obst_dist(float f) {obst_dist = f;}

	void set_ir_on(bool b) {ir_on = b;}
	void set_ir_send(bool b) {ir_send = b;}
	void set_imu_on(bool b) {imu_on = b;}
	void set_imu_gyro_send(bool b) {imu_gyro_send = b;}
	void set_imu_acc_send(bool b) {imu_acc_send = b;}
	void set_imu_comp_send(bool b) {imu_comp_send = b;}

	void set_motors_on(bool b) {motors_on = b;}
	void set_vel(float f) {vel = f;}

	void set_m1_kp(float f) {m1_kp = f;}
	void set_m1_ki(float f) {m1_ki = f;}
	void set_m2_kp(float f) {m2_kp = f;}
	void set_m2_ki(float f) {m2_ki = f;}
	void set_th_kp(float f) {th_kp = f;}
	void set_th_ki(float f) {th_ki = f;}

	void set_fwd_dist(float f) {fwd_dist = f;}
	void set_trn_deg(float f) {trn_deg = f;}
	void set_trn_r(float f) {trn_r = f;}
	void set_servo(float f) {servo = f;}

	void set_debug(bool b) {debug = b;}

	void set_fwd(bool b) {fwd = b;}
	void set_trn(bool b) {trn = b;}
	void set_trnr(bool b) {trnr = b;}
	void set_drive(bool b) {drive = b;}
	void set_followline(bool b) {followline = b;}

	void set_x_t(float f) {x_t = f;}
	void set_y_t(float f) {y_t = f;}
	void set_th_t(float f) {th_t = f;}

	// other methods
	String to_string();
	void debug_params();

//private:
	// Attributes:
	int action = -1;				// all actions >0; default=-1;
	bool connect = false;
	bool reset_enc = false;
	bool stop = false;
	bool avoid_obst = false;
	float obst_dist = 0.0;			// [mm]

	bool ir_on = false;
	bool ir_send = false;

	bool imu_on = false;
	bool imu_gyro_send = false;
	bool imu_acc_send = false;
	bool imu_comp_send = false;

	bool motors_on = false;
	float vel = false;

	float m1_kp = 1.0;
	float m1_ki = 1.0;

	float m2_kp = 1.0;
	float m2_ki = 1.0;

	float th_kp = 1.0;
	float th_ki = 1.0;

	float fwd_dist = 0.0;			// [mm]
	float trn_deg = 0.0;			// [ª]
	float trn_r = 0.0;				// [mm] turning radius
	float servo = 0.0;

	bool debug = false;				// useful now to debug on the TSY, but need to send essential info (params...) to the RPI to debug from there
	
	bool fwd = false;				// forward mode				// all might be grouped in one integer value (0=idle, 1=fwd, 2=trn, 3=trnr, 4=drive)
	bool trn = false;				// turn mode
	bool trnr = false;				// turn with radius mode
	bool drive = false;				// drive to target mode
	bool followline = false; 		// followline mode

	float x_t = 0.0;				// target position in Robot coord. syst.
	float y_t = 0.0;
	float th_t = 0.0;

	constexpr static int BUF_LEN = 100;
	char buf[BUF_LEN];
	int buf_length = 0;
	int buf_count = 0;
	bool new_msg = false;

	const float BIG_FLOAT = 191919.191919;
	const int BIG_INT = 2828;


	enum Actions {
		CONNECT,			// To know when the connection started and ended to send (or not) messages
		RESET_ENC,
		STOP,				// Immediatelly stops the driving when turned on (security)
		AVOID_OBSTACLES,	// If true, it will stop if IR detect and obstacle closer than a certain distance
							// else it will ignore any obstacle
		IR_ON,				// Enable/Disable reading IR data
		IR_SEND,			// Enable/Disable sending IR data
		
		IMU_ON,				// Enable/Disable reading IMU data
		IMU_GYRO_SEND,		// Enable/Disable sending IMU gyroscope data
		IMU_ACC_SEND,		// Enable/Disable sending IMU accelerometer data
		IMU_COMP_SEND,		// Enable/Disable sending IMU compass data

		MOTORS_ON,			// Enable/Disable motors
		DEBUG,				// Enable/Disable debugging messages

		// this are params, not actions
		
		SET_PID_M1,			// All 'SET_PID_...' need to be sent together with, at least one
		SET_PID_M2,			// of the following: 'kp', 'ki'
		SET_PID_TH,
		//...	

		FWD,
		TRN,
		TRNR,
		DRIVE,
		FOLLOW
	};


	struct Command {
		String A = "a";					// Action
		String B = "b";					// Value of the action
		String FWD = "fwd";				// Drive forward a certain distance [mm]
		String TRN = "trn";				// Turn certain degrees [º]
		String TRNR = "trnr";			// Turning radius [mm]
		String V = "v";					// Maximum speed [mm/s]
		String S = "s";					// Servo position [?]
		String OD = "od";				// Distance to detect obstacles [mm]
		String KP = "kp";				// P gain
		String KI = "ki";				// I gain
		String X_t = "xt";				// X  coord. of target pose (in robot coord. syst.)
		String Y_t = "yt";				// Y  coord. of target pose (in robot coord. syst.)
		String TH_t = "tht";			// Th coord. of target pose (in robot coord. syst.)

		String X_w = "xw";				// X  coord. of robot pose (in world coord. syst.)
		String Y_w = "yw";				// Y  coord. of robot pose (in world coord. syst.)
		String TH_w = "thw";			// Th coord. of robot pose (in world coord. syst.)
		String IR1 = "ir1";				// ir 1, sensor value
		String IR2 = "ir2";				// ir 2, sensor value
		String GYRO1 = "g1";			// gyroscope 1, sensor value
		String GYRO2 = "g2";			// gyroscope 2, sensor value
		String GYRO3 = "g3";			// gyroscope 3, sensor value
		String ACC1 = "a1";				// accelerometer 1, sensor value
		String ACC2 = "a2";				// accelerometer 2, sensor value
		String ACC3 = "a3";				// accelerometer 3, sensor value
		String COMP1 = "c1";			// compass 1, sensor value
		String COMP2 = "c2";			// compass 2, sensor value
		String COMP3 = "c3";			// compass 3, sensor value
		String OF = "of";				// obstacle found

	};


	// decode the received message into target
	void msg2params(); // OLD WAY: (String msg);

	// encode the target to send the message
	String sensorData2msg(double _odo[3], float _ir[2], int _imu_cmps[3], int _imu_gyro[3], int _imu_accel[3], bool _obstacle_found); // might need to get last data from sensors as input











	// OLD functions

	// have to go with struct target
	//void print_target(target new_pose);


	//void init_output_buffer();


	/*/ might go in an other place
	struct target{ // info RPi send to Teensy
		// tartet position in robot coord. syst.
	    float x;
	    float y;
	    float th;

	    // servo setpoint
	    float servo; // maybe not here?
	};*/


};
#endif