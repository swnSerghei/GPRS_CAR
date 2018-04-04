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
#include "uart.h"
#include "parkingLight.h"
#include "interrupts.h"

#define KEY1_ENABLE BIT2
#define KEY2_ENABLE BIT0
#define KEY2_DIAG   BIT7
#define KEY3_ENABLE BIT5
#define KEY3_DIAG   BIT6
#define STOPENGINEAFTER_X_TIMES_OVERFLOWS 20

#define PARKING_BRAKE   BIT5    //PORT x    OK-high
#define SHIFTER         BIT4    //PORT x    OK-high
#define ROTATION_PIN    BIT1    //port x

#define SLEEP_PERIODIC_AFTER 24
#define StayActive_EngineStart 20       //20min maximum 255 minute
uint8 counter_StayActive_EngineStart;
uint32 avgRotationSpeed;
bool OverFlowOcured;
uint32 avgRotationSpeedValidate;
uint8 OverRotationTimeValidation;
uint8 OverflowOcuredInEngineStarted;
uint8 NotActiveTime;
#define LEARNING_TIMES 3
#define OVER_ROTATION_TIME 3//if avg rotaion if over learning rotation speed 3 times validate starting car and stop to hold key3
#define OVERFLOW_TIME_BETWEEN_KEYS 250//25seconds
#define DELAY_AFTER_FAILED_START 50     //5 seconds
#define RETRY_TIMES 3                   //3 time retry to start the car
#define HOLD_KEY3_MORE 10
//#define SECOND 10   //10*100mS
uint8 retryersToStartEngine;
bool carLearn;
uint8 carLearnTimes;
uint8 timeBetween2Key_3Key;
uint8 tmpCounterForCalculateKeyTime;
uint8 waitKeyTimer;
uint8 holdKey3Time;
uint8 tmpKey3Time;

void reportMethod();
void keyPosition_init();
void startEngine();
void stopEngine();
void activateKey1();
void deactivateKey1();
bool parkingState();
bool shifterState();
void unlock();
bool key2_diag_fallingOcured1;
bool key2_diag_fallingOcured2;
enum LearnStates
{
  NotLearn=0,
  Learn
};
enum KeyStateMachine
{
    KeysOff=0,
    KeysOffFailed=1,
    Key1on=2,
    Key2on=4,
    Key3on=8,
    Key3fail1=16,
    key3fail2=32,
    motorStarted=64
};
uint8 KarStastes;

#endif /* LIBRARY_KEY_H_ */
