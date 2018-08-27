#include "Arduino.h"
#include "utils.h"

void setUpPowerPins(){
	pinMode(PIN_BATTERY_VOLTAGE,INPUT);
  	pinMode(PIN_POWER_ROBOT,OUTPUT);
  	pinMode(PIN_POWER_IR,OUTPUT);
  	
  	digitalWrite(PIN_POWER_ROBOT, HIGH);
  	digitalWrite(PIN_POWER_IR, HIGH);
}

void checkBattery(){
	// Check if battery voltage is low, if true, power robot off
	float batt=analogRead(PIN_BATTERY_VOLTAGE);
	batt = batt*16.2/1.2/1024*3.3;// /1024*12;
	Serial.print("\nBattery [V]: ");
	Serial.println(String(batt));

	if (batt < 10){
		Serial.println("Low battery!");
		digitalWrite(PIN_POWER_ROBOT, LOW);
		digitalWrite(PIN_POWER_IR, LOW);
	}
	else if(batt > 12){ // Maybe just else, but be careful!!!!
		digitalWrite(PIN_POWER_ROBOT, HIGH);
		digitalWrite(PIN_POWER_IR, HIGH);
	}
}
