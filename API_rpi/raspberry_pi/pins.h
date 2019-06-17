#ifndef PINS_H
#define PINS_H

#define USING_RPI
// To define raspi pins used

// to check the pins run this command on the Pi: gpio readall
// leds 				WiringPi | BCM | Physical | Color used
#define LED_KA 				22	// 	 6 |	31	  |	  green
#define LED_LEADER 			23	// 	13 |	33	  |	  yellow
#define LED_PLAN_NAV 		24	// 	19 |	35	  |	  blue
#define LED_TASK 			25	// 	26 |	37	  |	  red

#define LED_TEST 			 7	// 	 4 |	 4	  |	 	- 

#endif