#include "fight.h"

void makeAMove(state states, tof tofs)
{
    uint16_t L_pwm, R_pwm;
    int L_T_dist, R_T_dist; //L_T_LastDist, R_T_LastDist;
    /*int L_S_dist, R_S_dist;// L_S_LastDist, R_S_LastDist; //<-- S oznacza bok*/
    int time0;
    int randomNumber;

    L_T_dist = states.GetD(0);
    R_T_dist = states.GetD(1);
    /*L_S_dist = states.GetD(2);
    R_S_dist = states.GetD(3);*/

   

    if (blackOnGround(states, tofs))
    {

        switch (states.GetDSH())
        {
        case state::nothing: // else - czyli brak wykrycia mozna dac tu cos typu szukanie przeciwnika or idk
        
        digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
           time0 = millis();
            randomNumber = random(3); // losowanie reakcji
            //Serial.println(randomNumber);
            while (randomNumber == 0 && (states.GetDSH() == state::nothing) && (millis() - time0 < TIME_OF_SEARCH_ROT) && blackOnGround(states, tofs) == true)
            {
                states.sensorsRead(tofs);
               
                L_pwm = 0;
                R_pwm = PWM_MAX;
                motorsForward(L_pwm, R_pwm);
                
            } // reakcja skret w lewo
            while (randomNumber == 1 && (states.GetDSH() == state::nothing) && (millis() - time0 < TIME_OF_SEARCH_ROT) && blackOnGround(states, tofs) == true)
            {
                states.sensorsRead(tofs);
                
                L_pwm = PWM_MAX;
                R_pwm = 0;
                motorsForward(L_pwm, R_pwm);
                
            } // reakcja skret w prawo
            if (randomNumber >= 2  && blackOnGround(states, tofs) == true)
            {
                states.sensorsRead(tofs);
                L_pwm = PWM_DRIVE;
                R_pwm = PWM_DRIVE;
                motorsForward(L_pwm, R_pwm);
                
            } // reakcja prosto
            
            /*L_pwm = PWM_MAX;
            R_pwm = PWM_MAX;*/
            //motorsForward(PWM_MAX, PWM_MAX);
            //digitalWrite(LED_GREEN, LOW);
            //digitalWrite(LED_RED, LOW);
            //L_pwm = PWM_DRIVE;
             //   R_pwm = PWM_DRIVE;
              // motorsForward(L_pwm, R_pwm);
            break;

        case state::left_right_tof: // R_F & L_F // najwazniejszy case jazda do przodu, jakiegos PID tu czy cos //PIZDA do przodu a nie pid!
            digitalWrite(LED_GREEN, HIGH);
            digitalWrite(LED_RED, HIGH);
            
                L_pwm = PWM_MAX;
                R_pwm = PWM_MAX;
                motorsForward(L_pwm, R_pwm);
                //Serial.println("left right tof");
            
            break;

        case state::left_sharp: // L_S
            //time1 = millis();
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, HIGH);
 
                
                R_pwm = PWM_MAX;
                L_pwm = 0;//constrain(map(L_S_dist,700,SHARP_BORDER,0,127),0,127);
                motorsForward(L_pwm, R_pwm);
                states.sensorsRead(tofs);        
                //Serial.println("left sharp");
           
            break;

        case state::left_tof: // L_T
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, HIGH);
            L_pwm = map(L_T_dist, 0, TOF_BORDER, 0,255);
            R_pwm = PWM_MAX;
            motorsForward(L_pwm, R_pwm);    
            //Serial.println("left  tof");  
            break;

        case state::right_tof: // R_T

            digitalWrite(LED_GREEN, HIGH);
            digitalWrite(LED_RED,LOW);

            R_pwm = map(R_T_dist, 0, TOF_BORDER, 0,255);
            L_pwm = PWM_MAX;
            
            motorsForward(L_pwm, R_pwm);
            //Serial.println(" right tof");
            break;

        case state::right_sharp: // R_S
            //time1 = millis();
            digitalWrite(LED_GREEN, HIGH);
            digitalWrite(LED_RED, LOW);
         
                L_pwm = PWM_MAX;
                R_pwm = 0; //constrain(map(R_S_dist,700,SHARP_BORDER,0,127),0,127);
                motorsForward(L_pwm, R_pwm);
                states.sensorsRead(tofs);       
    
            //Serial.println("right sharp");
            break;

      default:
        digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
            L_pwm = PWM_DRIVE;
                R_pwm = PWM_DRIVE;
                motorsForward(L_pwm, R_pwm);    
                //Serial.println("default");   
            break;
        }

    //Serial.print(L_pwm);
    //Serial.print(" chuj ");
    //Serial.println(R_pwm);
    }

    
}

bool blackOnGround(state states, tof tofs)
{
    bool L_G, R_G;
    uint16_t time0;
    L_G = states.GetG(0);
    R_G = states.GetG(1);

    if (L_G == 0 && R_G == 0)
    {
//digitalWrite(LED_RED, LOW);
        return true; // <-- nie ma bialej linii, można jechać! :)
        

    }
    else
    {
       // digitalWrite(LED_RED, HIGH);
        if (L_G == 1 && R_G == 1) // <-- biala linia na obydwu
        {
            
            time0 = millis();
            motorsHardBrake();
            motorsBack();
            delay(TIME_OF_BACK*2);
            while ((states.GetDSH() == state::nothing) && (millis() - time0 < TIME_OF_WHITE_ROT))
            {

                states.sensorsRead(tofs);
                motorsLeftRotation();
                
            }

            
        }

        else if (L_G == 0 && R_G == 1) // <-- biala linia po prawej
        {
            
            time0 = millis();
            motorsHardBrake();
            motorsBack();
            delay(TIME_OF_BACK);
            while ((states.GetDSH() == state::nothing) && (millis() - time0 < TIME_OF_WHITE_ROT))
            {
                states.sensorsRead(tofs);
                motorsLeftRotation();
                
            }
        }

        else if (L_G == 1 && R_G == 0) // <-- biala linia po lewej
        {
           
            time0 = millis();
            motorsHardBrake();
            motorsBack();
            delay(TIME_OF_BACK);
            while ((states.GetDSH() == state::nothing) && (millis() - time0 < TIME_OF_WHITE_ROT))
            {
                states.sensorsRead(tofs);
                motorsRightRotation();
                
            }
        }
        motorsBackOFF();
        motorsForward(PWM_MAX, PWM_MAX);
        return false;
    }
}
