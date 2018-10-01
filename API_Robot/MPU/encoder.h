#ifndef encoder_h
#define encoder_h

#include "Arduino.h"

class Encoder
{
public:
	Encoder();
	Encoder(int pinA, int pinB);
};

#endif