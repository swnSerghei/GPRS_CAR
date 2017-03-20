/*
 * task.c
 *
 *  Created on: 20 мар. 2017 г.
 *      Author: Serghei
 */
#include "task.h"
void task1s()
{
    seconds++;
    if (seconds == 40 && wakeUpReason == 0)//wakeup and read message f present any command
    {
        wakeUpReason = wakeUpByTimer; wakeUp();
    }
    if (seconds == 60)
    {
        seconds=0;minutes++;
        if (wakeUpReason == wakeUpByTimer) go_to_sleep();
    }
    if (minutes == 60)
    {
        minutes=0;hours++;
    }
}
void go_to_sleep()
{
    #ifdef debugMode
    putInUartBuffer("GO in sleep Mode\n");
    sendUartBuffer();
    __delay_cycles(100000);
    #endif
    wakeUpReason = sleepMode;
    _BIS_SR(LPM1_bits + GIE); // Enter LPM3 w/interrupt
}
void wakeUp()
{
    __bic_SR_register(LPM3_bits); // exits LPM3
}
