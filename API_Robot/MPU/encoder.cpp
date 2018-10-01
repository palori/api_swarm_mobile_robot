
#include "Arduino.h"
#include "encoder.h"
#include "Encoder.h"
#include "pins.h"

encoder::encoder(){}
_Encoder=Encoder(0,0);

encoder::encoder(int pinA, int pinB){
	_Encoder=Encoder(pinA, pinB);
}

encoder::get(){
  return _Encoder;
}



