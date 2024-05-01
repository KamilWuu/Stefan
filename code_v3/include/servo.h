#include "defines.h"
//servo_set_position(2900,2100); //90stopni dol
//servo_set_position(1000,4000); //0stopni gora

// Maksymalna wartość licznika Timera 1 (ustawiona na podstawie taktowania)
#define MAX_COUNT 40000

void servoInit();
void servosFight();
void servosWait();
void servoDisable();