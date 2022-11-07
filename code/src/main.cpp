// ILYT_MS V.3

#include <Arduino.h>
#include "setup.h"
#include "drive.h"




void debugTest()
{
  delay(1000);
  digitalWrite(LED_GREEN, LOW);
  delay(300);
  digitalWrite(LED_GREEN, HIGH);
  delay(300);
  digitalWrite(LED_GREEN, LOW);
  delay(300);
  digitalWrite(LED_GREEN, HIGH);
  delay(300);
  digitalWrite(LED_GREEN, LOW);
  delay(2000);
  digitalWrite(LED_GREEN, HIGH);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool ifstart()
{
 


    if (digitalRead(STARTER_SIGNAL) == 1)
    {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      return true;
    }
    else
    {
      return false;
    }
  




}

/**
 * @brief
 *
 * @param Sensor
 * @param numberOfSamples
 * @return int
 */
int tryBreakfast(unsigned int Sensor, unsigned int numberOfSamples)
{

  int samples[numberOfSamples + 2];
  int result, sample_max = 0, sample_min = 2000, sum = 0;

  for (unsigned int i = 0; i < numberOfSamples + 2; i++)
  {
    samples[i] = analogRead(Sensor);
    if (samples[i] > sample_max)
    {
      sample_max = samples[i];
    }
    if (samples[i] < sample_min)
    {
      sample_min = samples[i];
    }
    sum += samples[i];
  }
  result = (sum - sample_min - sample_max) / (numberOfSamples);
  return result;
}



void sensorPrint(int left, int right, String tmp)
{
  Serial.print(left);
  Serial.print("\t <-- Lewy \t Prawy  --> \t ");
  Serial.print(right);
  Serial.print("\t\t");
  Serial.println(tmp);
}

bool ifWhiteLine()
{
  int G_value_Left, G_value_Right;

  G_value_Left = analogRead(LEFT_GROUND_SENSOR);
  G_value_Right = analogRead(RIGHT_GROUND_SENSOR);
  sensorPrint(G_value_Left, G_value_Right, "ifwhiteline");

  if(G_value_Left < GROUND_DIFFERENCE && G_value_Right < GROUND_DIFFERENCE+150)
  {
    motorsBack(400);
    motorsLeft(1500);
    motorsRight(700);
  }

  if (G_value_Left < GROUND_DIFFERENCE)
  {

   
    motorsBack(300);
    motorsRight(1500);
    

    return true;
  }
  if (G_value_Right < GROUND_DIFFERENCE+150)
  {

    
    motorsBack(300);
    motorsLeft(1500);
    return true;
  }
  else
  {
    return false;
  }
}

void searchOpponent(String found)
{


  if (found == "FORWARD" || found == "NOTHING")
  {
    if(ifWhiteLine() == false)
    {
    motorsForward();
    }
  }
  if (found == "LEFT")
  {

    motorsLeft(1000);
    
  }
  if (found == "RIGHT")
  {

    motorsRight(1000);
    
  }

}

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



/**
 * @brief
 *
 * @param L
 * @param R
 * @return String
 */
String foundDirection(int L, int R)
{
  if (L > LEFT_DISTANCE_BORDER && R < RIGHT_DISTANCE_BORDER)
  {
    return "LEFT";
  }
  if (L < LEFT_DISTANCE_BORDER && R > RIGHT_DISTANCE_BORDER)
  {
    return "RIGHT";
  }
  if (L > LEFT_DISTANCE_BORDER && R > RIGHT_DISTANCE_BORDER)
  {
    return "FORWARD";
  }
  else
  {
    return "NOTHING"; // FORWARD??
  }
}

int main(void)
{
  /////////Arduino stuff//////////
  init();
#if defined(USBCON)
  USBDevice.attach();#endif
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

  Serial.begin(9600);

  /**
   *
   *
   *
   *
   *
   *
   **/

  int R_distance = 0, L_distance = 0;
  String LastFound = "NOTHING", rotate = "left";
  Serial.begin(9600);
  Serial.print("start_test");
  for(int i = 0; i < 5; i++)
  {
    
    analogWrite(LED_RED, 20);
    digitalWrite(LED_GREEN, LOW);
    delay(80);
    analogWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    delay(80);
  }
  digitalWrite(LED_GREEN, LOW);
  
  while (ifstart() == false)
  { 
   
    analogWrite(LED_RED, 20);
    
      if (digitalRead(START_BUTTON) == 0)
      {
        rotate = "right";
        digitalWrite(LED_GREEN, HIGH);
        delay(200);
        digitalWrite(LED_GREEN, LOW);
      }


    
  }


  analogWrite(LEFT_PWM, LEFT_PWM_MAX);
  analogWrite(RIGHT_PWM, RIGHT_PWM_MAX);
  
  

  if (rotate == "left")
  {
    motorsLeft(3000);
  }

  if (rotate == "right")
  {
    motorsRight(3000);
  }



  for (;;)
  { // void loop() equivalent
    

    if(digitalRead(STARTER_SIGNAL) == 0)
    {
      motorsLow();
      analogWrite(LEFT_PWM, 0);
      analogWrite(RIGHT_PWM, 0);
      digitalWrite(LED_GREEN, LOW);
      analogWrite(LED_RED, 20);
      delay(1000);
      analogWrite(LED_RED, 0);
      delay(1000);
      continue;
    }

    R_distance = analogRead(RIGHT_DIST_SENSOR);
    L_distance = analogRead(LEFT_DIST_SENSOR);
    motorsForward();


    LastFound = foundDirection(L_distance, R_distance);

    // JAZDA NOWY SPOSOB

    if (L_distance > LEFT_DISTANCE_BORDER || R_distance > RIGHT_DISTANCE_BORDER)
    {
      if (ifWhiteLine() == false)
      {
        int difference, left_pwm, right_pwm;
        difference = abs(L_distance - R_distance);
        if (difference > 165)
        {   analogWrite(LEFT_PWM, LEFT_PWM_VALUE);
            analogWrite(RIGHT_PWM, RIGHT_PWM_VALUE);
          if (L_distance > R_distance)
          {
            left_pwm = constrain((LEFT_PWM_VALUE - (L_distance / 6)), 25, 255);
            right_pwm = constrain((RIGHT_PWM_VALUE + (L_distance / 6)), 25, 255);
          }

          if (R_distance > L_distance)
          {
            left_pwm = constrain((LEFT_PWM_VALUE + (R_distance / 6)), 25, 255);
            right_pwm = constrain((RIGHT_PWM_VALUE - (R_distance / 6)), 25, 255);
          }
        }
        else
        {
          left_pwm = LEFT_PWM_MAX;
          right_pwm = RIGHT_PWM_MAX;
        }

        analogWrite(LEFT_PWM, left_pwm);
        analogWrite(RIGHT_PWM, right_pwm);
        motorsForward();
      }
    }

    // SZUKANIE PRZECIWNIKA

    if (L_distance < LEFT_DISTANCE_BORDER && R_distance < RIGHT_DISTANCE_BORDER)
    { 
      analogWrite(LEFT_PWM, LEFT_PWM_MAX);
      analogWrite(RIGHT_PWM, RIGHT_PWM_MAX);
      if (ifWhiteLine() == false)
      {
        searchOpponent(LastFound);
      }
    }

    ////////////Arduino stuff/////////////
    if (serialEventRun)
      serialEventRun();
    //////////////////////////////////////
    
  }
  return 0;
}