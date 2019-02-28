
#include <comm_1.h>

int LED_PIN = 13;                         //pin in Teensy 3.0
int BLINK_FREQ = 5;
int count = 0;
time_t pctime = 0;
/*const int BUF_LEN = 100;
char buf[BUF_LEN];
int buf_count = 0;
bool new_msg = false;*/

COMM_TSY comm_tsy;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  char a[10] = {'@','h','o','l','a','$','\0'};
  String b = String(a);
  Serial.println("a: "+b+", len: "+String(b.length()));
}

void loop() {
  // put your main code here, to run repeatedly:
  //getNumbersFromSerial();                       // working example
  led_blink(LED_PIN, BLINK_FREQ);
  //getCommandsFromSerial();
  
  comm_tsy.read_serial();
  
}


/*************
   LED BLINK
 *************/
void led_blink(int led_pin, int blink_freq) {
  //Serial.println("led_blink");
  digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(blink_freq);             // wait for a second
  digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
  delay(blink_freq);             // wait for a second
}


// This version is more robust
boolean getNumbersFromSerial() {
  
  if (count < 100) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '@') {
        pctime = 0;  // always begin anew when '@' received
        count = 0;
      }
      else {
        if (c >= '0' && c <= '9') {
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number
          count++;
        }
      }
    }
  }
  Serial.println("pc time: "+String(pctime)+", count: "+String(count));
  //DateTime.sync(pctime);   // Sync clock to the time received
}

/*/ This version is more robust
String getCommandsFromSerial() {
  
  if (buf_count < BUF_LEN) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c=='@') {
        buf_count = 0;            // always begin anew when '@' received
        buf[buf_count] = '\0';    // reset the buffer
        new_msg = true;
      }
      else if (new_msg && c=='$'){
        buf[buf_count] = '\0';
        new_msg = false;
        Serial.println("buf: "+String(buf));
        return String(buf);
      }
      else if (new_msg){
        buf[buf_count] = c;
        buf_count++;
      }
      else if (buf_count > BUF_LEN){
        buf_count = 0;
        new_msg = false;
      }
    }
  }
  Serial.println("buf: "+String(buf)+", new: "+String(new_msg));
  return "";
}*/
