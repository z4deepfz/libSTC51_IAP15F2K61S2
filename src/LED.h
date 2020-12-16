#ifndef LED_H//
#define LED_H


#include "stc51.h"
#include "functions.h"

#define ENABLE_AUTO_PAUSE_LED
#ifdef ENABLE_AUTO_PAUSE_LED
    static const unsigned LEDdelay = 20;
#endif // ENABLE_AUTO_PAUSE_LED

/*
#define StopSeg()       P0=0
#define StartLED()      P23=1
#define StopLED()       P23=0
#define updateValue(v)  P0=NumOnSeg[v]
#define StopP0()        P0=0
#define UpdateLED()     StartLED(); P0=ledbuf
*/

static __code unsigned NumOnSeg[18]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x40,0x00};
static __code unsigned NumNegFlag = 1;

bool flagAutoRemoveDot = false;
static __pdata unsigned segbuf[8] = {0,0,0,0,0,0,0,0};
static __pdata bool     dotbuf[8] = {0,0,0,0,0,0,0,0};
static unsigned ledbuf = 0;

static void selectSeg(const unsigned short pos) { P2 = P2 & 0xF8 | pos; }
static void StopP0() { P0 = 0; }
void StartLED() { P23 = 1; }
void StopLED()  { P23 = 0; }
void StopSeg()  { P0 = 0; }
void updateValue(unsigned v) { P0 = NumOnSeg[v]; }
void SetSegNumber(const unsigned short pos, const unsigned value) { segbuf[pos] = value; }

void UpdateLED()
{
    StartLED();
    P0 = ledbuf;
}

static void updateSingleSegShow(const unsigned short pos, const unsigned value)
{
    // stop all level on P0(showing vector)
    StopP0();

    // select such segbit
    selectSeg(pos);

    // update single seg
    updateValue(value);

    // add dot
    if(dotbuf[pos]){
        P0 |= 1<<7;
        if(flagAutoRemoveDot){
            dotbuf[pos] = false;
        }
    }
}

void SetLEDValue(const unsigned value)
{
    ledbuf = value;
}

void UpdateSeg()
{
    StopLED();
    for(unsigned short i=0; i<8; ++i){
        updateSingleSegShow(i, segbuf[i]);
#ifdef ENABLE_AUTO_PAUSE_LED
        for(unsigned j=0; j<LEDdelay; ++j){}
#endif // ENABLE_AUTO_PAUSE_LED
    }
    StopP0();

    return;
}

void UpdateAllLED()
{
    StopLED();
    UpdateSeg();
    UpdateLED();
#ifdef ENABLE_AUTO_PAUSE_LED
    for(unsigned i=0; i<200; ++i);
#endif // ENABLE_AUTO_PAUSE_LED
    StopLED();
    StopP0();
    return;
}

void FillTubeNumber(signed long value)
{
    const __bit f = value<0;
    if(f){
        value = -value;
    }
    for(int i=7; i>=0; --i){
        segbuf[i] = value % 10;
        value /= 10;
    }
    if(f){
        segbuf[0] = 16;
    }
}

void AddDot(const unsigned pos) { dotbuf[pos] = true; }
void Deldot(const unsigned pos) { dotbuf[pos] = false; }
void ClearDot()
{
    for(unsigned short i=0; i<8; ++i){
        dotbuf[i] = false;
    }
}

void KeepSegEmpty()
{
    for(unsigned short i=0; i<8; ++i){
        segbuf[i] = 17;
    }
}

void SetSegHL(long high4, long low4)
{
    const __bit fh = high4<0;
    const __bit fl = low4<0;
    if(fh) high4 = - high4;
    if(fl) low4  = - low4;
    for(int i=3; i>=0; --i){
        segbuf[i] = high4 % 10;
        high4 /= 10;
    }
    for(int i=7; i>=4; --i){
        segbuf[i] = low4 % 10;
        low4 /= 10;
    }
    if(fh) segbuf[0]=16;
    if(fl) segbuf[4]=16;
}

#endif // LED_H
