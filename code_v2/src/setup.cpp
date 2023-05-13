#include "setup.h"

void pinModes()
{

    pinMode(START_BUTTON,INPUT);
    pinMode(STARTER_SIGNAL,INPUT);

    pinMode(RIGHT_DIST_SENSOR,INPUT);
    pinMode(RIGHT_GROUND_SENSOR,INPUT);
    pinMode(LEFT_DIST_SENSOR,INPUT);
    pinMode(LEFT_GROUND_SENSOR,INPUT);

    pinMode(XSHUT_1, OUTPUT);
    pinMode(XSHUT_2, OUTPUT);

    pinMode(LEFT_PWM, OUTPUT);
    pinMode(LEFT_BACK, OUTPUT);
    pinMode(LEFT_FORWARD, OUTPUT);

    pinMode(RIGHT_PWM, OUTPUT);
    pinMode(RIGHT_BACK, OUTPUT);
    pinMode(RIGHT_FORWARD, OUTPUT);

    pinMode(SERVO_LEFT, OUTPUT);
    pinMode(SERVO_RIGHT, OUTPUT);
    
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    
}