#include <msp430g2955.h>
#include "library/printf.h"
#include "library/uart.h"
#include "library/interrupts.h"
#include "library/commontypes.h"
#include "library/system.h"
#define debugMode 0
/*
 * main.c
 */

void main(void)
{
    watchDogConfigure();
    configureFrequency();
    Init_Uart();
    initTimerOnlyOnPowerOn();
    go_to_sleep();
    __enable_interrupt();
while(1)
    {

    }
}
