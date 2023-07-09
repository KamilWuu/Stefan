#pragma once


#include "setup.h"
#include "state.h"
#include "drive.h"
#include "Servo.h"

#define PWM_MAX 255
#define PWM_DRIVE 255
#define TIME_OF_WHITE_ROT 1000
#define TIME_OF_ATTACK_ROT 1000
#define TIME_OF_SEARCH_ROT 1000

#define TIME_OF_BACK 400



void makeAMove(state states, tof tofs);
bool blackOnGround(state states, tof tofs);