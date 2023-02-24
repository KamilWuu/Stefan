#include "setup.h"
#include "drive.h"
void motorsLow()
{
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACK, LOW);
  digitalWrite(RIGHT_BACK, LOW);
}

void motorsForward()
{

  digitalWrite(LEFT_FORWARD, HIGH);
  digitalWrite(RIGHT_FORWARD, HIGH);
}

void motorsBack(int time)
{
  motorsLow();
  digitalWrite(LEFT_BACK, HIGH);
  digitalWrite(RIGHT_BACK, HIGH);
  delay(time);
  motorsLow();
}

/**
 * @brief
 *
 * @param time
 */
void motorsLeft(int time)
{
  int R, L, time_tmp = 0;
  motorsLow();
  R = analogRead(RIGHT_DIST_SENSOR);
  L = analogRead(LEFT_DIST_SENSOR);

  while (R < RIGHT_DISTANCE_BORDER && L < LEFT_DISTANCE_BORDER && time_tmp < time)
  {

    digitalWrite(LEFT_BACK, HIGH);
    digitalWrite(RIGHT_FORWARD, HIGH);
    time_tmp++;
    R = analogRead(RIGHT_DIST_SENSOR);
    L = analogRead(LEFT_DIST_SENSOR);
    delay(1);
  }

  motorsLow();
}

void motorsRight(int time)
{
  int R, L, time_tmp = 0;
  motorsLow();
  R = analogRead(RIGHT_DIST_SENSOR);
  L = analogRead(LEFT_DIST_SENSOR);

  while (R < RIGHT_DISTANCE_BORDER && L < LEFT_DISTANCE_BORDER && time_tmp < time)
  {

    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_BACK, HIGH);
    time_tmp++;
    R = analogRead(RIGHT_DIST_SENSOR);
    L = analogRead(LEFT_DIST_SENSOR);
    delay(1);
  }

  motorsLow();
}

void motorsLeftS(int time)
{
  int R, L, time_tmp = 0;
  motorsLow();
  R = analogRead(RIGHT_DIST_SENSOR);
  L = analogRead(LEFT_DIST_SENSOR);

  while (R < RIGHT_DISTANCE_BORDER && L < LEFT_DISTANCE_BORDER && time_tmp < time && digitalRead(STARTER_SIGNAL) == 1)
  {

    digitalWrite(LEFT_BACK, HIGH);
    digitalWrite(RIGHT_FORWARD, HIGH);
    time_tmp++;
    R = analogRead(RIGHT_DIST_SENSOR);
    L = analogRead(LEFT_DIST_SENSOR);
    delay(1);
  }

  motorsLow();
}

void motorsRightS(int time)
{
  int R, L, time_tmp = 0;
  motorsLow();
  R = analogRead(RIGHT_DIST_SENSOR);
  L = analogRead(LEFT_DIST_SENSOR);

  while (R < RIGHT_DISTANCE_BORDER && L < LEFT_DISTANCE_BORDER && time_tmp < time && digitalRead(STARTER_SIGNAL) == 1)
  {

    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_BACK, HIGH);
    time_tmp++;
    R = analogRead(RIGHT_DIST_SENSOR);
    L = analogRead(LEFT_DIST_SENSOR);
    delay(1);
  }

  motorsLow();
}