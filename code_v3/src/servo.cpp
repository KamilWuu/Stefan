#include "servo.h"
//TIMER 1
void servoInit() {
    // Konfiguracja timera 1 w trybie Fast PWM z ICR1 jako maksymalną wartością licznika
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8

    // Ustawienie maksymalnej wartości licznika Timera 1
    ICR1 = MAX_COUNT;

    // Ustawienie pozycji neutralnych serwomechanizmów
    OCR1A = 1000; //servo_set_position(1000,4000); //0stopni gora
    OCR1B = 4000;
    //servo_set_position(1000,4000); //0stopni gora
    
}

void servosFight() {
    OCR1A = 2900;
    OCR1B = 2100;
    //servo_set_position(2900,2100); //90stopni dol
}
void servosWait() {
    OCR1A = 1000;
    OCR1B = 4000;
    //servo_set_position(1000,4000); //0stopni gora
}
void servoDisable(){
    TCCR1A = 0b00000000;
    TCCR1B = 0b00000000;
}