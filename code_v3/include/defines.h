#include <avr/io.h>
#include <util/delay.h>

//PINOUT DEFINITIONS

//USER FUNCTIONS
#define LED_WHITE PB5 ////d13
#define LED_RED PB4 ////d12

#define BUTTON PD2  ////d2
#define STARTER PD4 ////d4

//MOTORS
#define RM_BACK PB0 ////d8
#define RM_FORWARD PD7 ////d7

#define LM_FORWARD PB3 ////d11
#define LM_BACK PD3 ////d3

//MOTORS PWM TIMER0 
#define RM_PWM PD6 //OCR0A  //  d6
#define LM_PWM PD5 //OCR0B  //  d5

//SERVOS TIMER1
#define SERVO_LEFT PB1 //OCR1A  //  d9
#define SERVO_RIGHT PB2 //OCR1B   //  d10

//ADC
#define GROUND_R PC0 //PRAWY CZUJNIK PODLOZA
#define GROUND_L PC6 //LEWY CZUJNIK PODLOZA

#define DIST_R PC1 //PRAWY CZUJNIK ODLEGLOSCI
#define DIST_L PC2 //LEWY CZUJNIK ODLEGLOSCI

#define BAT_V PC3   //NAPIECIE NA BATERII

