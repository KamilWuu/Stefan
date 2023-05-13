#include "drive.h"

void motorsForward(uint8_t left_pwm, uint8_t right_pwm)
{
    analogWrite(LEFT_PWM, left_pwm);
    analogWrite(RIGHT_PWM, right_pwm);
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_FORWARD, HIGH);
}

void motorsOFF()
{   
    digitalWrite(LEFT_FORWARD, LOW);
    digitalWrite(RIGHT_FORWARD, LOW);
    digitalWrite(LEFT_BACK, LOW);
    digitalWrite(RIGHT_BACK, LOW);
}

void motorsHardBrake()
{
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_FORWARD, HIGH);
    digitalWrite(LEFT_BACK, HIGH);
    digitalWrite(RIGHT_BACK, HIGH);
}

void motorsLeftRotation()
{
    motorsOFF();
    digitalWrite(LEFT_BACK, HIGH);
    digitalWrite(RIGHT_FORWARD, HIGH);
    
}

void motorsRightRotation()
{
    motorsOFF();
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_BACK, HIGH);
}


void motorsBackOFF()
{
    digitalWrite(LEFT_BACK, LOW);
    digitalWrite(RIGHT_BACK, LOW);
}

void motorsBack()
{
    motorsOFF();
    digitalWrite(LEFT_BACK, HIGH);
    digitalWrite(RIGHT_BACK, HIGH);
}