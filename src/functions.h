#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "stc51.h"
#include "Timer.h"

// basic class
#define bool unsigned short
#define true 1
#define false 0

// keys
#define Key0 P32
#define Key1 P33
#define Key2 P17

//verbose
#define Verb P24
#define Verbing (Verb==0)

// macro functions
/*
#define EnableBeep()    P3M1 &= ~(1<<4);P3M0 |= 1<<4
#define EnableKeys()    P3M1 |= (1<<2)|(1<<3)|(1<<7); P34 = 0
#define EnableLED()     P0M1 = 0x00; P0M0 = 0xFF; P2M1 &= ~(1<<3); P2M0 |= 1<<3
#define BeepAct()       P34^=1
#define EnableVerbose() P2M1 |= 1<<4; P2M0 &= ~(1<<4)
#define ShowOnLED(v)    EnableLED(); P0=v
#define between(v,l,r)  ((v>=l) && (v<=r))
*/

__code unsigned DEBOUNCE_CNT = 50;


static bool between(const long v, const long l, const long r)
{
    return v>=l && v<=r;
}

void EnableBeep()
{
	P3M1 &= ~(1<<4);
	P3M0 |= 1<<4;
}

void EnableKeys()
{
    P3M1 |= (1<<2) | (1<<3) | (1<<7);
    P34 = 0;
}

void EnableLED()
{
    P0M1 = 0x00;
    P0M0 = 0xFF;
    P2M1 &= ~(1<<3);
    P2M0 |= 1<<3;
}

void BeepAct()
{
    P34 ^= 1;
}

void EnableVerbose()
{
    //verbose at P24
    P2M1 |= 1<<4;
    P2M0 &= ~(1<<4);
}

void ShowOnLED(unsigned v)
{
    EnableLED();
    P0 = v;
    return;
}

__bit IsKeyDown(__sbit __k)
{
    for(unsigned i=0; i<DEBOUNCE_CNT; ++i){
        if(__k == 1){
            return false;
        }
    }
    return true;
}

__bit IsKeyUp(__sbit __k)
{
    for(int i=0; i<DEBOUNCE_CNT; ++i){
        if(__k == 0){
            return false;
        }
    }
    return true;
}

__bit IsKey0Trigger()
{
    static bool f = true;
    if(IsKeyDown(Key0) && f){
        f = false;
        return true;
    }
    if(IsKeyUp(Key0)){
        f = true;
    }
    return false;
}

__bit IsKey1Trigger()
{
    static bool f = true;
    if(IsKeyDown(Key1) && f){
        f = false;
        return true;
    }
    if(IsKeyUp(Key1)){
        f = true;
    }
    return false;
}

__bit IsKey2Trigger()
{
    static bool f = true;
    if(IsKeyDown(Key2) && f){
        f = false;
        return true;
    }
    if(IsKeyUp(Key2)){
        f = true;
    }
    return false;
}

void WaitKey0Up()
{
    while(1){
        bool f = true;
        for(int i=0; i<DEBOUNCE_CNT; ++i){
            if(Key0 == 0){
                f = false;
                break;
            }
        }
        if(f == true){
            break;
        }
    }
}

void WaitKey1Up()
{
    while(1){
        bool f = true;
        for(int i=0; i<DEBOUNCE_CNT; ++i){
            if(Key1 == 0){
                f = false;
                break;
            }
        }
        if(f == true){
            break;
        }
    }
}

void WaitKey2Up()
{
    while(1){
        bool f = true;
        for(int i=0; i<DEBOUNCE_CNT; ++i){
            if(Key2 == 0){
                f = false;
                break;
            }
        }
        if(f == true){
            break;
        }
    }
}

#endif
