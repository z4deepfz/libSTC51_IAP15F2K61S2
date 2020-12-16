#ifndef ARROWKEY_H
#define ARROWKEY_H

//#define ENABLE_AD_HANDLE
#include "functions.h"
#include "AD.h"

enum ArrowKeyMode
{
    Idle, Left, Right, Up, Down, Press, K2, Unknown
};

static enum ArrowKey ArrKeyOn;
static unsigned long rawArrKey;

static enum ArrowKeyMode getArrowKeyStatus(const signed long res)
{
    if(between(res, 1000, 1024)){
        return Idle;
    }
    else if(between(res, 590, 610)){
        return Left;
    }
    else if(between(res, 145, 155)){
        return Right;
    }
    else if(between(res, 720, 750)){
        return Up;
    }
    else if(between(res, 300, 310)){
        return Down;
    }
    else if(between(res, 450, 460)){
        return Press;
    }
    else if(between(res, 0, 10)){
        return K2;
    }
    else{
        return Unknown;
    }
}

static void updateArrKeyStatus()
{
    ArrKeyOn = getArrowKeyStatus(rawArrKey);
}

#ifdef ENABLE_AD_HANDLE

void AD_handle() AD_INTERRUPTED
{
    static unsigned cnt = 0;
    static unsigned long buf = 0;
    const unsigned fcnt = 10;
    if(cnt >= fcnt){
        rawArrKey = buf / fcnt;
        updateArrKeyStatus();
        FillTubeNumber(rawArrKey);
        cnt = 0;
        buf = 0;
    }
    else{
        buf += GetADResValue();
        cnt++;
    }
    ResetAD();
    StartAD();
}

#endif // ENABLE_AD_HANDLE

void StartArrowKeyListening()
{
    EnableADInterrupt();
    setPinAsADMode(7); // P1.7
    SelectADChannel(ArrowKey);
    SetADRJ(0);
    ADPowerOn();
    StartAD();
}

void StopArrowKeyListening()
{
    setPinAsNormalMode(7);
    StopAD();
}

bool IsArrowKey(enum ArrowKeyMode __k)
{
    switch(ArrKeyOn){
        case Idle: return __k==Idle;
        case Left: return __k==Left;
        case Right: return __k==Right;
        case Up: return __k==Up;
        case Down: return __k==Down;
        case Press: return __k==Press;
        case K2: return __k==K2;
        default: return false;
    }
}



#endif // ARROWKEY_H
