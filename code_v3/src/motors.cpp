#include "motors.h"
//TIMER 0
//OCR0A RM_PWM
//OCR0B LM_PWM

void initMotorsPWM(){
    
    // Timer 0 configuration
    TCCR0A |= (1 << COM0A1) | (1 <<COM0B1) | (1 << WGM01) | (1 << WGM00);

    TCCR0B |= (1 << CS00);   // No prescaler*/
    
    OCR0A = 255;
    OCR0B = 255;
    
    
}

/*LEFT & RIGHT*/
void setMotorsSpeed(uint8_t LM_speed, uint8_t RM_speed){

    if(RM_speed < 0){OCR0A = 0;}
    else if(RM_speed > 255){OCR0A = 255;}
    OCR0A = RM_speed;

    if(LM_speed < 0){OCR0B = 0;}
    else if(LM_speed > 255){OCR0B = 255;}
    OCR0B = LM_speed;

}

void motorsForward(){   

    PORTB &= ~(1 << RM_BACK);
    PORTD &= ~(1 << LM_BACK);
    PORTD |= (1 << RM_FORWARD);
    PORTB |= (1 << LM_FORWARD);
}

void motorsBack(){

    PORTB &= ~(1 << LM_FORWARD );
    PORTD &= ~(1 << RM_FORWARD);
    PORTD |= (1 << LM_BACK);
    PORTB |= (1 << RM_BACK);
}

void motorsHardBrake(){

    PORTD |= (1 << LM_BACK) | (1 << RM_FORWARD);
    PORTB |= (1 << RM_BACK) | (1 << LM_FORWARD);
}

void motorsOFF(){
    PORTB &= ~(1 << LM_FORWARD );
    PORTD &= ~(1 << RM_FORWARD);
    PORTB &= ~(1 << RM_BACK);
    PORTD &= ~(1 << LM_BACK);
}

