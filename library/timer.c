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
    minutes = 0;
    hours   = 0;
    TA0CCR0  = 0xAFFF;                        //need to calibrate for 100ms
    TA0CTL  = TASSEL_2 + ID_3 + MC_1;
    TA0CCTL0 = CCIE;
}
