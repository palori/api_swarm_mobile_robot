#ifndef PINS_H
#define PINS_H


#define USING_ARDUINO // flag to see if need to include Arduino library or not
// Simplier version than the original from JCA.
// Teensy 3.5
//////////////////////////////////////////////////////////////////////////////////

// Pin Definitions for Regbot v4.1 2018

//Motor names

#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2

// Main pins
#define PIN_LED_STATUS          11
#define PIN_LED_DEBUG           13
#define PIN_START_BUTTON        26
#define PIN_LINE_LED_HIGH       12
#define PIN_LINE_LED_LOW        25
#define PIN_DISABLE2            7//27
#define PIN_POWER_IR            24
#define PIN_POWER_ROBOT         28

// ADC pins
#define ADC_NUM_NO_LS           5
#define ADC_NUM_ALL             (ADC_NUM_NO_LS + 8)
//
#define PIN_IR_RAW_1            A1
#define PIN_IR_RAW_2            A0
#define PIN_BATTERY_VOLTAGE     A14
#define PIN_LEFT_MOTOR_CURRENT  31//A2
#define PIN_RIGHT_MOTOR_CURRENT 32//A3
//
#define PIN_LINE_SENSOR_0       A12
#define PIN_LINE_SENSOR_1       A13
#define PIN_LINE_SENSOR_2       A15
#define PIN_LINE_SENSOR_3       A16
#define PIN_LINE_SENSOR_4       A17
#define PIN_LINE_SENSOR_5       A18
#define PIN_LINE_SENSOR_6       A19
#define PIN_LINE_SENSOR_7       A20

// Motor Controller pins
#define PIN_LEFT_DIR            37//6
#define PIN_LEFT_PWM            35//3
#define PIN_RIGHT_PWM           36//4
#define PIN_RIGHT_DIR           38//2

#define PIN_LEFT_ENCODER_A      23
#define PIN_LEFT_ENCODER_B      22
#define PIN_RIGHT_ENCODER_A     20
#define PIN_RIGHT_ENCODER_B     21
#define M1DIR           PIN_LEFT_DIR // M1IN2 - Teensy Digital (direction)
#define M1PWM           PIN_LEFT_PWM // M1IN1 - Teensy Digital (PWM)
#define M2DIR3          PIN_RIGHT_DIR // M2IN2 - Teensy Digital (direction) hardware 3 only
#define M2PWM3          PIN_RIGHT_PWM // M2IN1 - Teensy Digital (PWM) hardware 3
#define M12DIS          PIN_DISABLE2 // M1+M2 D2  enable both motors - hardware 3 only
#define M1ENC_A         PIN_LEFT_ENCODER_A // Encoder A - Teensy Digital
#define M1ENC_B         PIN_LEFT_ENCODER_B // Encoder B - Teensy Digital
#define M2ENC_A         PIN_RIGHT_ENCODER_A // Encoder A - Teensy Digital
#define M2ENC_B         PIN_RIGHT_ENCODER_B // Encoder B - Teensy Digital
// Legacy pins (only used < 3)
#define M1DIS1          4 // M1D2  - Teensy Digital 4 (disable (hiz))) - hardware < 3
#define M2DIS1          10 // M2D2  - Teensy Digital 10 (disable (hi-z)) - hardware < 3
#define M2PWM1          9 // M2IN1 - Teensy Digital 9 (PWM) hardware < 3
#define SLEW            7 // SLEW  - Teensy Digital 7  -  hardware 1 og 2 only, hardware 3 fixed high
#define M2DIR1          8 // M2IN2 - Teensy Digital 8 (direction) hardware < 3 only

// Servo pins
#define PIN_SERVO1      5 // Teensy pin for servo 1
#define PIN_SERVO2      9 // Teensy pin for servo 2
#define PIN_SERVO3      10 // Teensy pin for servo 3
#define PIN_SERVO4      29 // Teensy pin for servo 4
#define PIN_SERVO5      30 // Teensy pin for servo 5 (Shared with Buzzer!)
#define SERVO_PIN_0     PIN_SERVO4 // Deprecated!
#define SERVO_PIN_1     PIN_SERVO5 // Deprecated!
#define PIN_BUZZER		30 // Shared with SERVO5!

#endif
