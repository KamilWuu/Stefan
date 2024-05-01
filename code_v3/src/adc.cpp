#include "adc.h"



void initADC() {
    ADCSRA = (1 << ADEN)   // Uruchomienie przetwornika ADC
           | (1 << ADPS2)  // Ustawienie preskalera na 128
           | (1 << ADPS1)
           | (1 << ADPS0);

    ADMUX = (1 << REFS1);
}

uint16_t readGround_R(){ //PC0
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    
    ADCSRA |= (1 << ADSC); // Rozpoczęcie konwersji
    
    while (ADCSRA & (1 << ADSC)); // Oczekiwanie na zakończenie konwersji
    
    return ADC; // Odczytanie wyniku konwersji
}

uint16_t readGround_L(){ //PC6
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    ADMUX |= (1 << MUX2) | (1 << MUX1);

    ADCSRA |= (1 << ADSC); // Rozpoczęcie konwersji
    
    while (ADCSRA & (1 << ADSC)); // Oczekiwanie na zakończenie konwersji
    
    return ADC; // Odczytanie wyniku konwersji
}




uint16_t readDist_R(){ //PC1
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    ADMUX |= (1 << MUX0);

    ADCSRA |= (1 << ADSC); // Rozpoczęcie konwersji
    
    while (ADCSRA & (1 << ADSC)); // Oczekiwanie na zakończenie konwersji
    
    return ADC; // Odczytanie wyniku konwersji
}

uint16_t readDist_L(){ //PC2
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    ADMUX |= (1 << MUX1);
    
    ADCSRA |= (1 << ADSC); // Rozpoczęcie konwersji
    
    while (ADCSRA & (1 << ADSC)); // Oczekiwanie na zakończenie konwersji
    
    return ADC; // Odczytanie wyniku konwersji
}

uint16_t readVoltage(){ //PC3
    

    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    ADMUX |= (1 << MUX1) | (1 << MUX0);
    
    ADCSRA |= (1 << ADSC); // Rozpoczęcie konwersji
    
    while (ADCSRA & (1 << ADSC)); // Oczekiwanie na zakończenie konwersji

    return ADC; // Odczytanie wyniku konwersji
}




// Function to update state and covariance based on a new measurement
void updateKalman(struct Sensor* sensor, float z) {
    // Predict state
    float x_pred = sensor->x;
    float P_pred = sensor->P + sensor->Q;
    
    // Calculate Kalman gain
    float K = P_pred / (P_pred + sensor->R);
    
    // Correct state
    sensor->x = x_pred + K * (z - x_pred);
    
    //Correct covariance
    sensor->P = (1 - K) * P_pred;
}

// Function for filtering measurements
uint32_t sensorFilter(struct Sensor* sensor, uint16_t (*readSensor)()) {
    uint32_t average = 0;
    
    for (uint32_t i = 0; i < NUM_MEASUREMENTS; i++) {
        uint32_t measurement = readSensor();
        
        // Kalman filter updates state based on the measurement
        updateKalman(sensor, (float)measurement);
        
        // Read the value after filtering
        uint32_t filteredValue = (uint32_t)sensor->x;
        
        // Add the filtered measurement to the average
        average += filteredValue;
    }
    
    // Calculate the average of the filtered measurements
    average /= NUM_MEASUREMENTS;
    
    return average;
}