
int PowerRobot=28;

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
  
  digitalWrite(right_dir_pin,LOW);
  digitalWrite(left_dir_pin,LOW);
  digitalWrite(PowerRobot,HIGH);
  
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Speed 0 to 255");
} 
 
 
void loop() 
{ 
  if (Serial.available())
  {
    speed = Serial.parseInt();
    
    if (speed >= 0 && speed <= 255)
    {
      Serial.print(String(speed));
      analogWrite(right_pwm_pin, speed);
      Serial.println(" -");
    }
  }
}
