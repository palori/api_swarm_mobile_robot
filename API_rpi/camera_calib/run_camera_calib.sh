g++ -ggdb `pkg-config --cflags opencv` camera_calibration.cpp  `pkg-config --libs opencv` -o cc