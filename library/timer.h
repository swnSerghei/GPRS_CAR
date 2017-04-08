/*
 * timer.h
 *
 *  Created on: 20 мар. 2017 г.
 *      Author: Serghei
 */

#ifndef LIBRARY_TIMER_H_
#define LIBRARY_TIMER_H_
#include "commontypes.h"
#include <msp430g2955.h>
void initTimerOnlyOnPowerOn();
uint32 timerRegisterOnLearning;
uint8 seconds;
uint8 minuts;
uint16 hours;
uint8 wakeupTimer;
uint8 whaitTimer;

#endif /* LIBRARY_TIMER_H_ */
