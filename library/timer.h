/*
 * timer.h
 *
 *  Created on: 20 ���. 2017 �.
 *      Author: Serghei
 */

#ifndef LIBRARY_TIMER_H_
#define LIBRARY_TIMER_H_
#include "commontypes.h"
#include <msp430g2955.h>
void initTimerOnlyOnPowerOn();

uint8 seconds;
uint8 minutes;
uint16 hours;

#endif /* LIBRARY_TIMER_H_ */
