#include "setup.h"
#include "adc.h"
#include "motors.h"
#include "servo.h"
#include "uart.h"

#define MAXPWM 255
#define MIDPWM 128
#define R_BORDER 650
#define L_BORDER 650
#define DIFF_BORDER 200

#define TIME_START_ROTATION 3000
#define TIME_WHITE_LINE_ROTATION 1000
#define TIME_SEARCH_ROTATION 500
#define TIME_BACK 100

#define GROUND_BORDER 500

#define MAX_NO_FOUND_TIME 3000

enum direction
{
  left,
  right
};

/*
// Funkcja do mrugania diodą
void blinkLED(uint32_t *last_blink_time, uint32_t blink_interval){
  uint32_t current_time = millis();

  if (current_time - *last_blink_time >= blink_interval){
    setPinState(&PORTB, LED_RED, 1);
    *last_blink_time = current_time;
  }

  if (current_time - *last_blink_time >= blink_interval / 2){
    setPinState(&PORTB, LED_RED, 0);
  }
}

void checkVoltage(uint32_t *lastblink){
  uint32_t sum = 0, voltage = 0;

  for (int i = 0; i < 5; i++){
    sum +=  readVoltage();
  }
  voltage = sum / 5;

  if (voltage < 820 && voltage >= 780){
    blinkLED(lastblink, 1000);
  }
  else if (voltage < 780){
    blinkLED(lastblink, 1000);
  }

}*/

/*void checkVoltage()
{
  uint32_t sum = 0, voltage = 0;

  for (int i = 0; i < 3; i++)
  {
    sum += readVoltage();
  }
  voltage = sum / 3;
  voltage = readVoltage();
  UART_SEND_STRING("NAPIECIE NA BATERII:");
  UART_SEND_INT(voltage);
  UART_SEND_STRING("\n");

  if (voltage < 760)
  {
    for (int i = 0; i < 5; i++)
    {
      setPinState(&PORTB, LED_RED, 1);
      _delay_ms(200);
      setPinState(&PORTB, LED_RED, 0);
      _delay_ms(100);
    }
  }
  else
  {
    for (int i = 0; i < 5; i++)
    {
      setPinState(&PORTB, LED_WHITE, 1);
      _delay_ms(200);
      setPinState(&PORTB, LED_WHITE, 0);
      _delay_ms(100);
    }
  }

  // 860 = 8.4V,
  // 750 = 7,3V
}*/

void rotate(direction dir, uint32_t time_of_rotation, Sensor S_L, Sensor S_R)
{
  uint32_t dist_L = 0, dist_R = 0;
  uint32_t actual_time = millis();
  if (dir == direction::left)
  {
    setMotorsSpeed(0, MAXPWM);
  }
  else
  {
    setMotorsSpeed(MAXPWM, 0);
  }

  while (millis() - actual_time < time_of_rotation)
  {
    dist_L = sensorFilter(&S_L, readDist_L);
    dist_R = sensorFilter(&S_R, readDist_R);
    if (dist_L > L_BORDER || dist_R > R_BORDER)
    {
      break;
    }
  }
}

void blackGround(Sensor sensor_L, Sensor sensor_R)
{

  if (readGround_L() < GROUND_BORDER)
  {
    setMotorsSpeed(MAXPWM, MAXPWM);
    motorsBack();
    _delay_ms(TIME_BACK);
    motorsForward();
    rotate(right, TIME_WHITE_LINE_ROTATION, sensor_L, sensor_R);
  }
  else if (readGround_R() < GROUND_BORDER)
  {
    setMotorsSpeed(MAXPWM, MAXPWM);
    motorsBack();
    _delay_ms(TIME_BACK);
    motorsForward();
    rotate(left, TIME_WHITE_LINE_ROTATION, sensor_L, sensor_R);
  }
}

int main(void)
{

  init();
  initADC();
  initMotorsPWM();
  servoInit();
  millis_init();
  USART_INIT(USART_PORT_0, BaudRate_115200);

  bool buttonState = 1, no_found_rot_state = 1;

  struct Sensor sensor_L = {0.001, 10, 0, 1}; // For distance sensor L
  struct Sensor sensor_R = {0.001, 10, 0, 1}; // For distance sensor R

  uint32_t dist_L = 0;
  uint32_t dist_R = 0;

  uint32_t diff = 0;
  uint8_t L_PWM = MAXPWM, R_PWM = MAXPWM;

  uint32_t last_no_found_time = 0;

  servosWait();
  for(int i = 0; i < 5; i++){
    setPinState(&PORTB, LED_WHITE, 1);
    setPinState(&PORTB, LED_RED, 0);
    _delay_ms(100);
    setPinState(&PORTB, LED_WHITE, 0);
    setPinState(&PORTB, LED_RED, 1);
    _delay_ms(100);
  }
  setPinState(&PORTB, LED_RED, 0);
  //_delay_ms(100);
  //checkVoltage();

  // waiting for start loop

  bool led_state = 0;

  while (readStarterState() == 0)
  {
    if (readButtonState() == 0)
    {
      buttonState = !buttonState;
      led_state = !led_state;
      setPinState(&PORTB, LED_WHITE, led_state);
    }
  }

  servosFight();
  motorsForward();
  setPinState(&PORTB, LED_RED, 1);
  rotate((direction)buttonState, TIME_START_ROTATION, sensor_L, sensor_R);

  while (1)
  {
    if (readStarterState() == 1)
    {
      // ground_L = readGround_L();//sensorFilter(&sensor_G_L, readGround_L);
      // ground_R = readGround_R();//sensorFilter(&sensor_G_R, readGround_R);

      dist_L = sensorFilter(&sensor_L, readDist_L);
      dist_R = sensorFilter(&sensor_R, readDist_R);
      if (dist_R > R_BORDER || dist_L > L_BORDER) // WYKRYCIE CZEGOKOLWIEK
      {
        diff = abs(dist_L - dist_R);
        if (diff < DIFF_BORDER) // WYKRYCIE NA OBYDWU
        {
          R_PWM = MAXPWM;
          L_PWM = MAXPWM;
        }
        else
        {
          if (dist_R > dist_L) // WYKRYCIE NA PRAWYM
          {
            L_PWM = MAXPWM;
            R_PWM = (255 - ((MAXPWM * dist_R) / 1023));
          }
          else // WYKRYCIE NA LEWYM
          {
            R_PWM = MAXPWM;
            L_PWM = (255 - ((MAXPWM * dist_L) / 1023));
          }
        }
      }
      else // BRAK WYKRYCIA
      {
        uint32_t no_found_time = millis();

        // Sprawdzenie, czy minął interwał
        if (no_found_time - last_no_found_time >= MAX_NO_FOUND_TIME)
        {
          // Zapisz bieżący czas jako poprzedni znacznik czasu
          last_no_found_time = no_found_time;
          // Wykonaj swoje zadanie
          rotate((direction)no_found_rot_state, TIME_SEARCH_ROTATION, sensor_L, sensor_R);
          no_found_rot_state = !no_found_rot_state;
        }
        R_PWM = MIDPWM;
        L_PWM = MIDPWM;
      }
      setMotorsSpeed(L_PWM, R_PWM);
      blackGround(sensor_L, sensor_R);
      // UART_SEND_MEASUREMENTS(dist_L, dist_R, L_PWM, R_PWM);
    }
    else
    {
      motorsOFF();
      setMotorsSpeed(0, 0);
      setPinState(&PORTB, LED_WHITE, 1);
      setPinState(&PORTB, LED_RED, 0);
      _delay_ms(500);
      setPinState(&PORTB, LED_WHITE, 0);
      setPinState(&PORTB, LED_RED, 1);
      _delay_ms(100);
    }
  }
}
