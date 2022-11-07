
#include "drive.h"
#include "fight.h"
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



bool ifWhiteLine()
{
  int G_value_Left, G_value_Right;

  G_value_Left = analogRead(LEFT_GROUND_SENSOR);
  G_value_Right = analogRead(RIGHT_GROUND_SENSOR);


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