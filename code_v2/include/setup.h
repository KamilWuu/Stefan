#pragma once

#include <Arduino.h>

#define XSHUT_2 4

#define XSHUT_1 2

#define SERVO_RIGHT 11
#define SERVO_LEFT 10

#define LED_GREEN 13
#define LED_RED 12

#define STARTER_SIGNAL A1
#define START_BUTTON A0

#define RIGHT_DIST_SENSOR A3
#define LEFT_DIST_SENSOR A2

#define LEFT_GROUND_SENSOR A6
#define RIGHT_GROUND_SENSOR A7

#define TIME_OF_START_ROT 2000 // 2000

#define RIGHT_BACK 6
#define RIGHT_FORWARD 5
#define RIGHT_PWM 3

#define LEFT_PWM 9
#define LEFT_BACK 8
#define LEFT_FORWARD 7

void pinModes();
