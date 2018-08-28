
int PowerRobot=28;
int disable_motors=27;

int left_pwm_pin=3;
int right_pwm_pin=4;
int left_dir_pin=6;
int right_dir_pin=2;

int speed=0;

void setup() 
{ 
  pinMode(right_pwm_pin, OUTPUT);
  pinMode(right_dir_pin, OUTPUT);
  pinMode(left_pwm_pin, OUTPUT);
  pinMode(left_dir_pin, OUTPUT);
  
  pinMode(PowerRobot, OUTPUT);
  pinMode(disable_motors, OUTPUT);
  
  digitalWrite(right_dir_pin,HIGH);
  digitalWrite(left_dir_pin,HIGH);
  digitalWrite(right_pwm_pin,HIGH);
  digitalWrite(left_pwm_pin,HIGH);
  
  digitalWrite(PowerRobot,HIGH);
  digitalWrite(disable_motors,HIGH);

  analogWriteFrequency(right_pwm_pin, 20000);
  analogWriteFrequency(left_pwm_pin, 20000);
  analogWriteResolution(12);
  
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Speed 0 to 4096");
} 
 
 
void loop() 
{ 
  if (Serial.available())
  {
    speed = Serial.parseInt();
    
    if (speed >= 0 && speed <= 4096)
    {
      Serial.print(String(speed));
      analogWrite(right_pwm_pin, speed);
      analogWrite(left_pwm_pin, speed);
      Serial.println(" -");
    } else {
      //digitalWrite(right_pwm_pin, LOW);  
    }
  }
}
