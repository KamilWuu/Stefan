
#include <Arduino.h>
#include "setup.h"
#include "Servo.h"
#include "state.h"
#include "fight.h"



int main(void)
{
  /////////Arduino stuff//////////
  init();
#if defined(USBCON)
  USBDevice.attach();
#endif
  ////////////////////////////////
  // SETUP
  //Serial.begin(9600);
  Wire.begin();
  //Serial.println("print test");
  state sensorsState;
  tof tofs;
  Servo servoLeft, servoRight;
  bool ifDetached = false;
  bool leftRotate = false;
  bool ledState = LOW;
  int time0, time1;
  pinModes();
  
  tofs.setID();

  for (int i = 0; i < 5; i++)
  {

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    delay(80);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    delay(80);
  }

  servoLeft.attach(SERVO_LEFT);
  servoRight.attach(SERVO_RIGHT);
  servoRight.write(130); // pozycja górna wachlarzy
  servoLeft.write(50);

  while(!(sensorsState.readStarter()))
  {
     digitalWrite(LED_RED, HIGH);
     if(sensorsState.readButton() == 0)
     {
      
      delay(100);
      if(sensorsState.readButton() == 0)
      {
        leftRotate = !leftRotate;
        ledState = !ledState;
      }
     }
     digitalWrite(LED_GREEN, ledState);
  }
  
  servoRight.write(30); // pozycja dolna wachlarzy
  servoLeft.write(150);
  time0 = millis();
  
  analogWrite(LEFT_PWM, PWM_MAX);
  analogWrite(RIGHT_PWM, PWM_MAX);

  if(leftRotate==true)
  {
    
      time0 = millis();
      motorsLeftRotation();
      while ((sensorsState.GetDSH() == state::nothing) && (millis() - time0 < TIME_OF_START_ROT))
      {
          sensorsState.sensorsRead(tofs);
          motorsLeftRotation();
      }

      motorsOFF();
  }
  else
  {
    
    time0 = millis();
      motorsRightRotation();
      while ((sensorsState.GetDSH() == state::nothing) && (millis() - time0 < TIME_OF_START_ROT))
      {
          sensorsState.sensorsRead(tofs);
          motorsRightRotation();
      }

      motorsOFF();
  }

  motorsForward(PWM_MAX,PWM_MAX);
  for (;;)
  {
    
    
    while (sensorsState.readStarter())
    {

    if (ifDetached == false)
    {
      time1 = millis();
      if (time1 - time0 > 1000)
      {
        servoRight.detach();
        servoLeft.detach();
        ifDetached = true;
      }
    }
    

    
    sensorsState.sensorsRead(tofs);
    makeAMove(sensorsState, tofs);
    //sensorsState.distSensorsTest();
    sensorsState.resetStates();
    



    }

    
    motorsOFF();
      digitalWrite(LED_RED,HIGH);
      digitalWrite(LED_GREEN,LOW);
      delay(300);
      digitalWrite(LED_RED,LOW);
      digitalWrite(LED_GREEN,HIGH);
      delay(300);

      

   

    ////////////Arduino stuff/////////////
    if (serialEventRun)
      serialEventRun();
    //////////////////////////////////////
  }
  return 0;
}