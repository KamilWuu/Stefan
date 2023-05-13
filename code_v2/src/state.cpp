#include "state.h"
  


state::state()
{   
    
    this->groundTab[0] = 0;
    this->groundTab[1] = 0;

    this->buttonState = 0;
    

    for(int i = 0; i < 2; i++)
    {
        this->distTab[i] = ZERO_DISTANCE;
     
    }
    this->distTab[2] = SHARP_ZERO_DISTANCE;
    
    this->distTab[3] = SHARP_ZERO_DISTANCE;
   


    this->starterState = 0;
    this->distStateHolder = nothing;
}


void state::sensorsRead(tof tof_sensors)
{
    
    readGround();
    tof_sensors.readDistSensors(this->distTab);
    this->starterState = digitalRead(STARTER_SIGNAL);
    this->distState();
}


void state::readGround()
{
    if(analogRead(LEFT_GROUND_SENSOR) < GROUND_BORDER)
    {
        this->groundTab[0] = 1;
    }
    else
    {
        this->groundTab[0] = 0;
    }

    if(analogRead(RIGHT_GROUND_SENSOR) < GROUND_BORDER)
    {
        this->groundTab[1] = 1;
    }
    else
    {
        this->groundTab[1] = 0;
    }
}


bool state::readButton()
{
    return this->buttonState = digitalRead(START_BUTTON);
}

bool state::readStarter()
{
    return this->starterState = digitalRead(STARTER_SIGNAL);
}



void state::distState() //Ustawienie "flagi" 
{
    if(L_TOF < TOF_BORDER && R_TOF < TOF_BORDER){distStateHolder = left_right_tof;} //5
    else if(R_TOF < TOF_BORDER){distStateHolder = right_tof;} //3
    else if(L_TOF < TOF_BORDER){distStateHolder = left_tof;} //2
    else if(R_SHARP > SHARP_BORDER){distStateHolder = right_sharp;} //4
    else if(L_SHARP > SHARP_BORDER){distStateHolder = left_sharp;} //1
    else{distStateHolder = nothing;}//0
}

void state::distSensorsTest()
{
  Serial.print("STATUS-> ");
  Serial.println(distStateHolder);
  
    Serial.print(L_SHARP);
    Serial.print("<-- L_SHARP\t\t");

    Serial.print(L_TOF);
    Serial.print("<-- L_TOF\t\t");

    Serial.print(R_TOF);
    Serial.print("<-- R_TOF\t\t");

    Serial.print(R_SHARP);
    Serial.print("<-- R_SHARP\t\t");

    Serial.print(groundTab[0]);
    Serial.print("<-- G_0\t\t");

    Serial.print(groundTab[1]);
    Serial.print("<-- G_1\t\t");

    Serial.println();
  //delay(500);
}

void state::resetStates()
{
    this->groundTab[0] = 1023;
    this->groundTab[1] = 1023;

    
    

    for(int i = 0; i < 2; i++)
    {
        this->distTab[i] = ZERO_DISTANCE;
     
    }
    this->distTab[2] = SHARP_ZERO_DISTANCE;
    
    this->distTab[3] = SHARP_ZERO_DISTANCE;
    


    this->starterState = 0;
    this->distStateHolder = nothing;
}