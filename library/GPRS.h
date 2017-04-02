/*
 * GPRS.h
 *
 *  Created on: Mar 21, 2017
 *      Author: uidg6243
 */

#ifndef LIBRARY_GPRS_H_
#define LIBRARY_GPRS_H_
#include <msp430g2955.h>
#include "library/commontypes.h"
#include "wakeupByTimer.h"
#include "timer.h"
extern const uint8 *SMSCommands[];
extern const uint8 *GPRSCommands[];
#define PWR_GPRS BIT0
#define RST_GPRS BIT1
#define INT_GPRS BIT2
#define POWER_ON_BUTTON 25 //3 second
#define timeToInvalidate 10 //1 seconds



enum gprs_state_machine
{
    GPRS_SLEEP=0,
    GPRS_INIT,
    GPRS_READY,
    GPRS_BUSY
};
enum CommandsBits
{
    parkingLight_start=0,
    parkingLight_stop,
    key1_start,
    key1_stop,
    engine_start,
    engine_stop,
    recirculare_start,
    recirculare_stop,
    ac_start,
    ac_stop,
    luneta_start,
    luneta_stop,
    parbriz_start,
    pabriz_stop,
    ventilator_3pozitie,
    ventilator_4pozitie,
    ventilator_off,
    delaySeconds,
    NrOfSMSComands
};

uint8 countForEachCommand[NrOfSMSComands];
uint8 gprs_state_machine;
uint8 gprs_second;
uint8 gprs_minuts;
uint8 PresentAnyCommand;
bool  allCommandsExecuted;
uint8 counterExecutedCommands;
uint8 listOfCommandsToExecuting[NrOfSMSComands];
uint8 counterHowManyCommands;
bool needToSendSMS;
enum GPRS_commands
{
    SMS=0,
    CALL,
    totalGPRScomands
};
uint8 GPRSCommands_Counter[totalGPRScomands];

enum listOFtelephonNumbers
{
tel1,
tel2,
totalTelephoneNumbers
};
uint8 telephonNumbers_Counter[totalTelephoneNumbers];

enum topologyRead
{
    readGPRSCommand,
    readTelephonNumber,
    readSMScontent,
    telephoneCall
};
uint32 StateOfCommands;

void init_GPRS_only_on_Startup();
void powerON_GPRS();
void init_GPRS_only_on_PowerOn();
bool wait_gprs_response(uint8 *command,uint8 whaiteTime);
void wait_gprs_loop(void);
bool write_gprs_command(uint8 *command,uint8 *response,uint8 whaiteTime);
#endif /* LIBRARY_GPRS_H_ */
