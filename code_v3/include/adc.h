#include "defines.h"
#include <stdlib.h>

#define NUM_MEASUREMENTS 5 // Number of measurements, adjust as needed

// Structure to store parameters and variables for a sensor
struct Sensor {
    float Q; // Process noise value
    float R; // Measurement noise value
    float x; // State variable
    float P; // State covariance
};

void initADC();
uint16_t readGround_R();
uint16_t readGround_L();

uint16_t readDist_R();
uint16_t readDist_L();

uint16_t readVoltage();


void updateKalman(struct Sensor* sensor, float z);
uint32_t sensorFilter(struct Sensor* sensor, uint16_t (*readSensor)());
    