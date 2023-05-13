#include "tof.h"

void tof::setID() // przepisac na rejestry!
{
  digitalWrite(XSHUT_1, HIGH);
  digitalWrite(XSHUT_2, HIGH);

  delay(50);
  digitalWrite(XSHUT_1, LOW);
  digitalWrite(XSHUT_2, LOW);

  delay(50);

  digitalWrite(XSHUT_1, HIGH);

  delay(50);
  // this->tofTab[] wszedzie
  tofTab[0].setAddress(TOF_1_ADDRESS);
  tofTab[0].setTimeout(TIMEOUT_VALUE);

  // while(!tofTab[0].init())
  //{
  //   Serial.println("0 ->jeszcze nie zainicjowane!");
  // }
  tofTab[0].init();
  delay(50);

  digitalWrite(XSHUT_2, HIGH);

  delay(50);

  tofTab[1].setAddress(TOF_2_ADDRESS);
  tofTab[1].setTimeout(TIMEOUT_VALUE);

  // while(!tofTab[1].init())
  //{
  //   Serial.println("1 ->jeszcze nie zainicjowane!");
  // }
  tofTab[1].init();
  delay(50);

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  for (int i = 0; i < 2; i++)
  {
    this->tofTab[i].setMeasurementTimingBudget(TIMING_BUDGET);
  }

#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  for (int i = 0; i < 4; i++)
  {
    tofTab[i].setMeasurementTimingBudget(200000);
  }
#endif
}

void tof::readDistSensors(int *dist)
{

  dist[0] = this->tofTab[0].readRangeSingleMillimeters();
  dist[1] = this->tofTab[1].readRangeSingleMillimeters();
  dist[2] = analogRead(LEFT_DIST_SENSOR);
  dist[3] = analogRead(RIGHT_DIST_SENSOR);
}

/*
void tof::tofTest(uint16_t *tab)
{

  if(tab[0] < DIST)
    {
      digitalWrite(13,HIGH); //RED
    }
    else
    {
      digitalWrite(13,LOW);
    }


    if(tab[1] < DIST)
    {
      digitalWrite(12,HIGH); //BLUE
    }
    else
    {
      digitalWrite(12,LOW);
    }

    if(tab[2] < DIST)
    {
      digitalWrite(11,HIGH); //SERVO_LEFT
    }
    else
    {
      digitalWrite(11,LOW);
    }

    if(tab[3] < DIST)
    {
      digitalWrite(3,HIGH); //SERVO_RIGHT
    }
    else
    {
      digitalWrite(3,LOW);
    }

}

void tof::setOne()
{
  digitalWrite(XSHUT_TOF_1,LOW);
  digitalWrite(XSHUT_TOF_2,LOW);
  digitalWrite(XSHUT_TOF_3,LOW);
  digitalWrite(XSHUT_TOF_4,LOW);

  digitalWrite(XSHUT_TOF_1,LOW);
  digitalWrite(XSHUT_TOF_2,LOW);
  digitalWrite(XSHUT_TOF_3,LOW);
  digitalWrite(XSHUT_TOF_4,HIGH);
  //this->tofTab[] wszedzie
  tofTab[3].setTimeout(TIMEOUT_VALUE);
  tofTab[3].setAddress(TOF_4_ADDRESS);
  tofTab[3].init();
  delay(10);

}

uint16_t tof::readOne()
{
  return this->tofTab[0].readRangeSingleMillimeters();
}
*/