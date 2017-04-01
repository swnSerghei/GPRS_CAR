/*
 * key.h
 *
 *  Created on: Mar 21, 2017
 *      Author: uidg6243
 */

#ifndef LIBRARY_KEY_H_
#define LIBRARY_KEY_H_
#include "commontypes.h"
#include <msp430g2955.h>
#include "GPRS.h"
#define KEY1_ENABLE BIT0
#define KEY2_ENABLE BIT1
#define KEY2_DIAG   BIT2
#define KEY3_ENABLE BIT3
#define KEY3_DIAG   BIT4
#define STOPENGINEAFTER_X_TIMES_OVERFLOWS 200

#define PARKING_BRAKE   BIT1    //PORT 3    OK-high
#define SHIFTER         BIT3    //PORT 3    OK-low
#define ROTATION_PIN    BIT3    //port 2

#define StayActive_EngineStart 20       //20min maximum 255 minute
uint8 counter_StayActive_EngineStart;
uint32 avgRotationSpeed;
bool OverFlowOcured;
uint32 avgRotationSpeedValidate;
uint8 OverRotationTimeValidation;
uint8 OverflowOcuredInEngineStarted;
#define LEARNING_TIMES 3
#define OVERFLOW_TIME_BETWEEN_KEYS 250//25seconds
#define DELAY_AFTER_FAILED_START 50     //5 seconds
#define RETRY_TIMES 3                   //3 time retry to start the car
#define HOLD_KEY3_MORE 30
uint8 retryersToStartEngine;
bool carLearn;
uint8 carLearnTimes;
uint8 timeBetween2Key_3Key;
uint8 tmpCounterForCalculateKeyTime;
uint8 waitKeyTimer;
uint8 holdKey3Time;
uint8 tmpKey3Time;

void keyPosition_init();
void startEngine();
void stopEngine();
void activateKey1();
void deactivateKey1();
bool parkingState();
bool shifterState();
enum LearnStates
{
  NotLearn=0,
  Learn
};
enum KeyStateMachine
{
    KeysOff=0,
    KeysOffFailed,
    Key1on,
    Key2on,
    Key3on,
    Key3fail1,
    key3fail2,
    motorStart
};
uint8 KarStastes;

#endif /* LIBRARY_KEY_H_ */
