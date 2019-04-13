#include "Arduino.h"
#include "utils.h"
 
using namespace std;

void setUpPowerPins(){
	pinMode(PIN_BATTERY_VOLTAGE,INPUT);
  	pinMode(PIN_POWER_ROBOT,OUTPUT);
  	pinMode(PIN_POWER_IR,OUTPUT);
  	pinMode(PIN_LED_STATUS,OUTPUT);
  	pinMode(M12DIS,OUTPUT);
  	
  	digitalWrite(PIN_POWER_ROBOT, HIGH);
  	digitalWrite(PIN_POWER_IR, HIGH);
}

float checkBattery(){
	// Check if battery voltage is low, if true, power robot off
	float batt=analogRead(PIN_BATTERY_VOLTAGE);
	batt = batt*16.2/1.2/1024*3.3;// /1024*12;
	//Serial.print("\nBattery [V]: ");
	//Serial.println(String(batt));

	if (batt < 10){
		//Serial.println("Low battery!");
		digitalWrite(PIN_POWER_ROBOT, LOW);
		digitalWrite(PIN_LED_STATUS, LOW);
		
	}
	else if (batt<11.5){
		digitalWrite(PIN_POWER_ROBOT, HIGH);
		digitalWrite(PIN_LED_STATUS, LOW);
		
	}
	else { //if(batt > 12){ // Maybe just else, but be careful!!!!
		digitalWrite(PIN_POWER_ROBOT, HIGH);
		digitalWrite(PIN_LED_STATUS, HIGH);
		
	}
	return batt;
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

