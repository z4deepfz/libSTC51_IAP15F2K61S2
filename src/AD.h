#ifndef AD_H
#define AD_H

#include "functions.h"

#define CHS(x) ADC_CONTR=(ADC_CONTR & 0xF8) | x
#define LIGHTSENSOR_PIN 4
#define TEMPERATURE_PIN 3
#define ARROWKEY_PIN 7

/*
#define setPinAsADMode(pin)     P1ASF|=1<<pin
#define setPinAsNormalMode(pin) P1ASF&=~(1<<pin)
#define getADRJ()               ((CLK_DIV>>5)&1)
#define SetADRJ(v)              CLK_DIV = CLK_DIV & (~(1<<5)) | ((v&1)<<5)
#define ADPowerOn()             ADC_CONTR |= 1<<7
#define ADPowerOff()            ADC_CONTR &= ~(1<<7)
#define IsADPowerOn()           (ADC_CONTR>>7)
#define StartAD()               ADC_CONTR |= 1<<3
#define StopAD()                ADC_CONTR &= ~(1<<3)
#define ADFinished()            (ADC_CONTR >> 4)
#define ResetADFinished()       ADC_CONTR &= ~(1<<4)
#define ResetAD()               ResetADFinished(); SetADResValue(0)
#define SetADSpeed(sp)          ADC_CONTR = ADC_CONTR & (~(3<<5)) | (sp<<5)
*/

enum ADChannel
{
    c0,
    c1,
    Hall,
    Temperature,
    Light,
    c5,
    c6,
    ArrowKey
};

static void setPinAsADMode(unsigned short pin) { P1ASF |= 1<<pin; }
static void setPinAsNormalMode(unsigned short pin) { P1ASF &= ~(1<<pin); }
static unsigned getADRJ() { return (CLK_DIV>>5)&1; }
void SetADRJ(unsigned short v) { CLK_DIV = CLK_DIV & (~(1<<5)) | ((v&1)<<5); }
void ADPowerOn() { ADC_CONTR |= 1<<7; }
void ADPowerOff() { ADC_CONTR &= ~(1<<7); }
bool IsADPowerOn() { return ADC_CONTR>>7; }
// before start ad, you should turn on the power first
void StartAD() { ADC_CONTR |= 1<<3; }
void StopAD() { ADC_CONTR &= ~(1<<3); }
void SetADSpeed(unsigned char sp) { ADC_CONTR = ADC_CONTR & (~(3<<5)) | (sp<<5); }

void SetADResValue(unsigned long value)
{
    if(getADRJ() == 1){
        /// 1: ADC_RES[1:0]存放高2位，ADC_RESL[7:0]存放低8位
        ADC_RESL = value & 0xff;
        ADC_RES  = (value>>8) & 0x3;
    }
    else{
        /// 0: ADC_RES[7:0]存放高8位，ADC_RESL[1:0]存放低2位ADC
        ADC_RESL = value & 0x3;
        ADC_RES  = (value>>2) & 0xff;
    }
    return;
}

unsigned long GetADResValue()
{
    if(getADRJ() == 1){
        unsigned long high2 = ADC_RES & 0x3;
        unsigned long low8 = ADC_RESL;
        return (high2<<8) | low8;
    }
    else{
        unsigned long high8 = ADC_RES;
        unsigned long low2 = ADC_RESL & 0x3;
        return (high8<<2) | low2;
    }
}

bool ADFinished() { return ADC_CONTR >> 4; }
void ResetADFinished() { ADC_CONTR &= ~(1<<4); }
void ResetAD() { ResetADFinished(); SetADResValue(0); }

void SelectADChannel(enum ADChannel c)
{
    switch(c){
        case c0: CHS(0); break;
        case c1: CHS(1); break;
        case Hall: CHS(2); break;
        case Temperature: CHS(3); break;
        case Light: CHS(4); break;
        case c5: CHS(5); break;
        case c6: CHS(6); break;
        case ArrowKey: CHS(7); break;
    }
    return;
}

#endif // AD_H
