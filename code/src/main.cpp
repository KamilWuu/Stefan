// ILYT_MS V.3

#include <Arduino.h>
#include <Servo.h>
#include "setup.h"
#include "drive.h"
#include "fight.h"

/**
 * @brief
 *
 * @param borderInt
 */
void distSensorConfig(int borderInt)
{
  int R_distance = 0;
  int L_distance = 0;
  int R = 0, L = 0;
  for (;;)
  {
    R_distance = tryBreakfast(RIGHT_DIST_SENSOR, 30);
    L_distance = tryBreakfast(LEFT_DIST_SENSOR, 30);
    Serial.print(R_distance);
    Serial.print("\t\t");
    Serial.print(L_distance);
    Serial.print("\t\tRoznica: ");
    Serial.print(abs(R_distance - L_distance));
    Serial.print("\t\t");
    Serial.print("R: ");
    Serial.print(R);
    Serial.print("\t\t");
    Serial.print("L: ");
    Serial.print(L);

    Serial.println();
    delay(10);

    if (R_distance > RIGHT_DISTANCE_BORDER)
    {
      R++;
    }
    if (L_distance > LEFT_DISTANCE_BORDER)
    {
      L++;
    }
  }
}

void sensorPrint(int left, int right, String tmp)
{
  Serial.print(left);
  Serial.print("\t <-- Lewy \t Prawy  --> \t ");
  Serial.print(right);
  Serial.print("\t\t");
  Serial.println(tmp);
}

int main(void)
{
  /////////Arduino stuff//////////
  init();
#if defined(USBCON)
  USBDevice.attach();
#endif
  ////////////////////////////////
  // Buttons
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(STARTER_SIGNAL, INPUT);

  // Sensors
  pinMode(LEFT_DIST_SENSOR, INPUT);
  pinMode(LEFT_GROUND_SENSOR, INPUT);
  pinMode(RIGHT_DIST_SENSOR, INPUT);
  pinMode(RIGHT_GROUND_SENSOR, INPUT);

  // Left motor
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_BACK, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT);
  // Right motor
  pinMode(RIGHT_BACK, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);
  // Ledy
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  // Serwa
  pinMode(SERVO_LEFT, OUTPUT);
  pinMode(SERVO_RIGHT, OUTPUT);

  Serial.begin(9600);

  /**
   *
   *
   *
   *
   *
   *
   **/

  int R_distance = 0, L_distance = 0, time0, time1;
  String LastFound = "NOTHING", rotate = "left";
  Servo ServoLeft, ServoRight;
  Serial.begin(9600);
  Serial.print("start_test");
  bool ifDetached = false, ifAtached = false;

  for (int i = 0; i < 5; i++)
  {

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    delay(80);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    delay(80);
  }
  ServoLeft.attach(SERVO_LEFT);
  ServoRight.attach(SERVO_RIGHT);
  ServoRight.write(130); // pozycja górna wachlarzy
  ServoLeft.write(50);

  digitalWrite(LED_GREEN, LOW);

  while (ifstart() == false)
  {

    digitalWrite(LED_RED, HIGH);

    if (digitalRead(START_BUTTON) == 0)
    {
      rotate = "right";
      digitalWrite(LED_GREEN, HIGH);
      delay(200);
      digitalWrite(LED_GREEN, LOW);
    }
  }
  ServoRight.write(20); // pozycja dolna wachlarzy
  ServoLeft.write(160);
  time0 = millis();

  analogWrite(LEFT_PWM, LEFT_PWM_MAX);
  analogWrite(RIGHT_PWM, RIGHT_PWM_MAX);

  if (rotate == "left")
  {
    motorsLeftS(3000);
  }

  if (rotate == "right")
  {
    motorsRightS(3000);
  }

  for (;;)
  { // void loop() equivalent
    if (ifDetached == false)
    {
      time1 = millis();
      if (time1 - time0 > 1000)
      {
        ServoRight.detach();
        ServoLeft.detach();
        ifDetached = true;
      }
    }
    if (digitalRead(STARTER_SIGNAL) == 0)
    {
      motorsLow();
      analogWrite(LEFT_PWM, 0);
      analogWrite(RIGHT_PWM, 0);
      if (ifAtached == false)
        /*{
          ServoLeft.attach(SERVO_LEFT);
          ServoRight.attach(SERVO_RIGHT);
          ServoRight.write(130);       //pozycja górna wachlarzy
          ServoLeft.write(50);
          ifAtached = true;
        }*/
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      delay(1000);
      digitalWrite(LED_RED, LOW);
      delay(1000);
      continue;
    }

    R_distance = analogRead(RIGHT_DIST_SENSOR);
    L_distance = analogRead(LEFT_DIST_SENSOR);

    motorsForward();

    LastFound = foundDirection(L_distance, R_distance);

    // JAZDA NOWY SPOSOB

    /*if (L_distance > LEFT_DISTANCE_BORDER || R_distance > RIGHT_DISTANCE_BORDER)
    {*/
    if (ifWhiteLine() == false)
    {
      int difference, left_pwm, right_pwm;
      difference = abs(L_distance - R_distance);
      if (difference > 250)
      {

        if (R_distance > L_distance)
        {
          left_pwm = constrain((LEFT_PWM_VALUE + (R_distance / 6)), 0, 255);
          right_pwm = constrain((RIGHT_PWM_VALUE - (R_distance / 6)), 0, 255);
        }
        if (L_distance > R_distance)
        {
          left_pwm = constrain((LEFT_PWM_VALUE - (L_distance / 6)), 0, 255);
          right_pwm = constrain((RIGHT_PWM_VALUE + (L_distance / 6)), 0, 255);
        }
      }
      else
      {
        left_pwm = LEFT_PWM_MAX;
        right_pwm = RIGHT_PWM_MAX;
      }
      /*sensorPrint(L_distance, R_distance, "odleglosc");*/
      sensorPrint(left_pwm, right_pwm, "pwm");
      analogWrite(RIGHT_PWM, right_pwm);
      analogWrite(LEFT_PWM, left_pwm);

      motorsForward();
    }
    /*}*/

    // SZUKANIE PRZECIWNIKA

    /* if (L_distance < LEFT_DISTANCE_BORDER && R_distance < RIGHT_DISTANCE_BORDER)
     {
       analogWrite(LEFT_PWM, LEFT_PWM_MAX);
       analogWrite(RIGHT_PWM, RIGHT_PWM_MAX);
       if (ifWhiteLine() == false)
       {
         searchOpponent(LastFound);
       }
     }*/

    ////////////Arduino stuff/////////////
    if (serialEventRun)
      serialEventRun();
    //////////////////////////////////////
  }
  return 0;
}