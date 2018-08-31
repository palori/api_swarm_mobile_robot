#include <utils.h>
#include <vector>
//using namespace std;

//bool hasRead = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("TEST CSV");
}

void loop() {
  // put your main code here, to run repeatedly:
  //if(!hasRead){
    vector<vector<String>> data = parse2DCsvFile("test.csv");
    /*
    for (auto l : data) {
        for (auto x : l)
            //cout << x << " ";
            Serial.print(String(x) + " ");
        //cout << endl;
        Serial.print("\n");
    }*/
    //hasRead = true;
  //}
}
