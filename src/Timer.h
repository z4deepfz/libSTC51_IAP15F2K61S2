#ifndef TIMER_H
#define TIMER_H

#include "functions.h"
#include "interrupt.h"

#define TMOD54(x, y) (TMOD=TMOD & 0xCF | ((x==1)<<5) | (y==1)<<4)
#define TMOD10(x, y) (TMOD=TMOD & 0xCF | ((x==1)<<1) | (y==1))

enum TimerMode
{
    idle,
    r16,
    u16,
    r8
};

void InitTimer0() { EnableTimer0Interrupt(); }
void InitTimer1() { EnableTimer1Interrupt(); }

void SetTimer0Mode(enum TimerMode m)
{
    switch(m){
        /*case idle: {
            TMOD10(1, 1);
            break;
        }*/
        case r16: {
            TMOD10(0, 0);
            break;
        }
        case u16: {
            TMOD10(0, 1);
        }
        case r8: {
            TMOD10(1, 0);
        }
    }
}

void SetTimer1Mode(enum TimerMode m)
{
    switch(m){
        case idle: {
            TMOD54(1, 1);
            break;
        }
        case r16: {
            TMOD54(0, 0);
            break;
        }
        case u16: {
            TMOD54(0, 1);
        }
        case r8: {
            TMOD54(1, 0);
        }
    }
}

void SetTimer0Value(unsigned high8, unsigned low8)
{
    TH0 = high8;
    TL0 = low8;
}

void SetTimer1Value(unsigned high8, unsigned low8)
{
    TH1 = high8;
    TL1 = low8;
}

void SetTimer0Clock(unsigned long cnt_clock)
{
    unsigned long v = 65535 - cnt_clock;
    SetTimer0Value( v/256, v%256 );
}

void SetTimer1Clock(unsigned long cnt_clock)
{
    unsigned long v = 65535 - cnt_clock;
    SetTimer1Value( v/256, v%256 );
}

void StartTimer0() { TR0 = 1; }
void StopTimer0() { TR0 = 0; }

void StartTimer1() { TR1 = 1; }
void StopTimer1() { TR1 = 0; }


#endif // TIMER_H
