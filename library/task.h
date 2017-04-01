/*
 * task.h
 *
 *  Created on: Mar 21, 2017
 *      Author: uidg6243
 */

#ifndef LIBRARY_TASK_H_
#define LIBRARY_TASK_H_

#include "commontypes.h"
#include <msp430g2955.h>
#include "timer.h"
#include "key.h"
#include "interrupts.h"
#include "GPRS.h"
#include "wakeupByTimer.h"
#define WAKEUP_AFTER_X_SECONDS 120
#define STAY_AWAKE 60
uint8 systemState;

enum wakeUpReason
{
    sleepMode = 0,
    wakeUpByTimerState,
    wakeUpByKey2State
};
void task1s();
void task100ms();
void go_to_sleep();
void wakeUp();

#endif /* LIBRARY_TASK_H_ */
