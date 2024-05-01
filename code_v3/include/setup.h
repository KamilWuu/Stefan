#include "defines.h"
#include <avr/interrupt.h>

void init();
void setPinState(volatile uint8_t *port, uint8_t pin, bool state);
bool readButtonState();
bool readStarterState();

extern volatile uint32_t millis_count;

void millis_init();
uint32_t millis();