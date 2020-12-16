#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "functions.h"

#define TIMER1_INTERRUPTED __interrupt(3)
#define TIMER0_INTERRUPTED __interrupt(1)
#define AD_INTERRUPTED __interrupt(5)

/*
#define EnableMasterInterrupt()     EA=1
#define DisableMasterInterrupt()    EA=0
#define EnableTimer0Interrupt()     ET0=1
#define DisableTimer0Interrupt()    ET0=0
#define EnableTimer1Interrupt()     ET1=1
#define DisableTimer1Interrupt()    ET1=0
#define EnableADInterrupt()         EADC=1
#define DisableADInterrupt()        EADC=0
*/

void EnableMasterInterrupt() { EA = 1; }
void DisableMasterInterrupt() { EA = 0; }

// Timer 0
void EnableTimer0Interrupt() { EnableMasterInterrupt(); ET0 = 1; }
void DisableTimer0Interrupt() { ET0 = 0; }

// Timer 1
void EnableTimer1Interrupt() { EnableMasterInterrupt(); ET1 = 1; }
void DisableTimer1Interrupt() { ET1 = 0; }

// AD convertor
void EnableADInterrupt() { EnableMasterInterrupt(); EADC = 1; }
void DisableADInterrupt() { EADC = 0; }


#endif // INTERRUPT_H
