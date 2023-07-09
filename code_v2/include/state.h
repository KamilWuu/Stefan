#pragma once
#include <Arduino.h>
#include "tof.h"

#define GROUND_BORDER 700

#define TOF_BORDER 350

#define SHARP_BORDER 300
#define RIGHT_SHARP_BORDER 250
#define LEFT_SHARP_BORDER 250
#define SHARP_DIFERENCE 26

#define SHARP_ZERO_DISTANCE 0

    #define L_TOF distTab[0]
    #define R_TOF  distTab[1]
    #define L_SHARP distTab[2]
    #define R_SHARP distTab[3]


class state
{

    int distTab[4];     // indeks: 0 = PRAWY_bok, 1 - lewy bok,  2 - lewy przod, 3  - prawy przod
   

    bool groundTab[2]; // indeks: 0 - PRAWY, 1 -LEWY //bool: 1 = biala linia, 0 = brak bialej linii
    bool buttonState;
    bool starterState;

public:
    enum STANY
    {
        // nothing=0,
        // left_sharp = 1,
        // left_tof = 2,
        // middle_tof = 3,
        // right_tof = 4,
        // right_sharp = 5,
        // left_middle_tof = 6,
        // middle_right_tof = 7,
        // left_middle_right_tof = 8

        nothing = 0,
        left_sharp = 1,
        left_tof = 2,
        right_tof = 3,
        right_sharp = 4,
        left_right_tof = 5

    };

private:
    STANY distStateHolder; // schowek na flage z tofow, patrz plik: "dist_State_Cases.png"

public:
    state();
    //~state();

    void sensorsRead(tof tof_sensors);
    void readGround();
    bool readButton();
    bool readStarter();
    void distSensorsTest();

    void distState(); // Ustawienie "flagi" z tofow, patrz plik: "dist_State_Cases.png"

    int GetD(int index) { return distTab[index]; };


    bool GetG(int index) { return groundTab[index]; };
    bool GetBS() { return buttonState; };
    bool GetST() { return starterState; };
    STANY GetDSH() { return distStateHolder; };
    void resetStates();
};
