#include "setup.h"
#include "adc.h"

void init(){
       
    DDRB |= (1<<LED_RED) | (1<<LED_WHITE) | (1<<LM_FORWARD) | (1 << SERVO_RIGHT) | (1 << SERVO_LEFT)| (1<< RM_BACK); 
    DDRD |= (1<<RM_FORWARD) | (1<< LM_BACK) | (1 << RM_PWM) | (1<<LM_PWM);
}


void setPinState(volatile uint8_t *port, uint8_t pin, bool state){

    if(state == 1){
        *port |= (1<<pin);

    }else{
        *port &= ~(1 << pin);
    }
}

bool readButtonState(){
    return (PIND & (1 << BUTTON));
}

bool readStarterState(){
    return (PIND & (1 << STARTER));
}

volatile uint32_t millis_count = 0;

void millis_init() {
    // Konfiguracja timera 2 w trybie CTC (Clear Timer on Compare Match)
    TCCR2A = (1 << WGM21);
    
    // Ustawienie wartości rejestru Compare dla timera 2 (względem częstotliwości zegara)
    // Wartość 124 odpowiada około 1 ms przy zegarze 16 MHz i preskalerze 128
    OCR2A = 124;
    
    // Włączenie przerwań Compare Match dla timera 2
    TIMSK2 |= (1 << OCIE2A);
    
    // Ustawienie preskalera na 128, aby uzyskać dokładny czas w milisekundach
    TCCR2B = (1 << CS22) | (1 << CS20);
    
    // Włączenie globalnych przerwań
    sei();
}

uint32_t millis() {
    uint32_t millis_value;
    // Wyłącz przerwania na krótko, aby uniknąć konfliktu odczytu wartości
    cli();
    millis_value = millis_count;
    sei();
    return millis_value;
}

// Obsługa przerwania Compare Match dla timera 2
ISR(TIMER2_COMPA_vect) {
    millis_count++;
}

