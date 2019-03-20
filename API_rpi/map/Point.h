#ifndef point_h
#define point_h

class Point {

public:
	Point();
	Point(int id_number,double xc,double yc, double thc);
	int id;
	
	double x;
	double y;
	double th;
	/*
	double get_x(){return x;}
	double get_y(){return y;}
	double get_th(){return th;}

	double set_x(double d) {x = d;}
	double set_y(double d) {y = d;}
	double set_th(double d) {th = d;}
	*/

//private:
	
};

#endif
