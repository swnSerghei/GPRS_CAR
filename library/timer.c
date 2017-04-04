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
    TA1CTL  = TASSEL_2 + ID_2 + MC_1;
    TA1CCTL0 = CCIE;
////    P1DIR |= ROTATION_PIN;
//    P2SEL |= ROTATION_PIN;
    TA0CTL |= TASSEL_2 + ID_3 + TACLR;
//    //TA0CCTL0 |= CM_1 + CCIS_2 + SCS + CAP;
//    TA0CCTL1 |= CAP + CM_1 + CCIE + SCS + CCIS_0;    // TA0CCR1 Capture mode; CCI1A; Both                                        // Rising and Falling Edge; interrupt enable

}
