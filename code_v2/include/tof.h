#pragma once
#include <Arduino.h>
#include "Wire.h"
#include "VL53L0X.h"
#include "setup.h"

// DEFINE ADRESSES
#define TOF_1_ADDRESS 0x50
#define TOF_2_ADDRESS 0x54



#define HIGH_SPEED
//#define HIGH_ACCURACY

#define TIMING_BUDGET 33
#define TIMEOUT_VALUE 40

#define ZERO_DISTANCE 900



class tof
{
VL53L0X tofTab[2]; 


public:

void setID();
void readDistSensors(int *tab);
/*void tofTest(uint16_t *tab);
uint16_t readOne();
void setOne();*/
};


