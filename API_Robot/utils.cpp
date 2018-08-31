#include "Arduino.h"
#include "utils.h"

#include <vector>
#include <sstream> //istringstream
#include <iostream> // cout
#include <fstream> // ifstream
 
using namespace std;

void setUpPowerPins(){
	pinMode(PIN_BATTERY_VOLTAGE,INPUT);
  	pinMode(PIN_POWER_ROBOT,OUTPUT);
  	pinMode(PIN_POWER_IR,OUTPUT);
  	pinMode(M12DIS,OUTPUT);
  	
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
	} else if(batt > 12){ // Maybe just else, but be careful!!!!
		digitalWrite(PIN_POWER_ROBOT, HIGH);
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


 
/**
 * Reads csv file into table, exported as a vector of vector of Strings.
 * @param inputFileName input file name (full path).
 * @return data as vector of vector of Strings.
 */
vector<vector<String>> parse2DCsvFile(String inputFileName) {
 
    vector<vector<String> > data;
    ifstream inputFile(inputFileName);
    int l = 0;
 
    while (inputFile) {
        l++;
        String s;
        if (!getline(inputFile, s)) break;
        if (s[0] != '#') {
            istringstream ss(s);
            vector<String> record;
 
            while (ss) {
                String line;
                if (!getline(ss, line, ','))
                    break;
                try {
                    record.push_back(stof(line));
                }
                catch (const std::invalid_argument e) {
                    //cout << "NaN found in file " << inputFileName << " line " << l
                    //     << endl;
                    Serial.println("NaN found in file " + String(inputFileName) << " line " << String(l));
                    e.what();
                }
            }
 
            data.push_back(record);
        }
    }
 
    if (!inputFile.eof()) {
        //cerr << "Could not read file " << inputFileName << "\n";
        Serial.print("Could not read file " + String(inputFileName) + "\n");
        __throw_invalid_argument("File not found.");
    }
 
    return data;
}