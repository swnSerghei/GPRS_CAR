/*
 * task.h
 *
 *  Created on: 20 мар. 2017 г.
 *      Author: Serghei
 */
#include "commontypes.h"
#include <msp430g2955.h>
#include "timer.h"
uint8 wakeUpReason;
enum stateMachine
{
    sleepMode = 0,
    wakeUpByTimer,
    wakeUpByKey2
};
void task1s();
void go_to_sleep();
void wakeUp();
