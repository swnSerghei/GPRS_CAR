/*
 * timer.c
 *
 *  Created on: 20 мар. 2017 г.
 *      Author: Serghei
 */
#include "timer.h"
void initTimerOnlyOnPowerOn()
{
    seconds = 0;
    minuts = 0;
    hours   = 0;
    TA1CCR0  = 0xC035;                        //need to calibrate for 100ms
    TA1CTL  = TASSEL_2 + ID_3 + MC_1;
    TA1CCTL0 = CCIE;
}
