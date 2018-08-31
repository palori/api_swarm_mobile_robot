#include "Arduino.h"
#include "utils.h"

void setUpPowerPins(){
	pinMode(PIN_BATTERY_VOLTAGE,INPUT);
  	pinMode(PIN_POWER_ROBOT,OUTPUT);
  	pinMode(PIN_POWER_IR,OUTPUT);
  	pinMode(M12DIS,OUTPUT);
  	
  	digitalWrite(PIN_POWER_ROBOT, HIGH);
  	digitalWrite(PIN_POWER_IR, HIGH);
}

bool checkBattery(){
	// Check if battery voltage is low, if true, power robot off
	float batt=analogRead(PIN_BATTERY_VOLTAGE);
	batt = batt*16.2/1.2/1024*3.3;// /1024*12;
	Serial.print("\nBattery [V]: ");
	Serial.println(String(batt));

	if (batt < 10){
		Serial.println("Low battery!");
		digitalWrite(PIN_POWER_ROBOT, LOW);
		return false;
	} else { //if(batt > 12){ // Maybe just else, but be careful!!!!
		digitalWrite(PIN_POWER_ROBOT, HIGH);
		return true;
	}
}

void enableMotors(){
	digitalWrite(M12DIS,HIGH);
}

void disableMotors(){
	digitalWrite(M12DIS,LOW);
}

void enableIR(){
	digitalWrite(PIN_POWER_IR, HIGH);
}
void disableIR(){
	digitalWrite(PIN_POWER_IR, LOW);
}