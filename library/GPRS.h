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
#include "timer.h"
#include "uart.h"
#include "parkingLight.h"
#include "printf.h"
extern const uint8 *SMSCommands[];
extern const uint8 *GPRSCommands[];
#define PWR_GPRS BIT3
#define RST_GPRS BIT0
#define RTS_GPRS BIT7   //RTS from GPS
#define CTS_GPRS BIT3   //CTS from GPS
#define POWER_ON_BUTTON 25 //3 second
#define timeToInvalidate 10 //1 seconds
#define SizeOfMSG 141       //last element is '\0', but message is 140
uint8 SMScontent[SizeOfMSG];
uint8 SMScontent_Counter;
bool telephone_complete;

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
    unlock_doors,
    delaySeconds,
    //without verify who sender, directly verify if present any sms command =====> defined by howManyCommandsWithoutVerifySender
    report,
    gps_off,
    gps_on,
    gps_get,

    NrOfSMSComands
};
#define gps_get_From_Length 21 //last value is '\0'
uint8 gps_get_From[gps_get_From_Length];
uint8 gps_get_From_Counter;
#define howManyCommandsWithoutVerifySender  4

uint8 countForEachCommand[NrOfSMSComands];
uint8 gprs_state_machine;
uint8 PresentAnyCommand;
bool  allCommandsExecuted;
uint8 counterExecutedCommands;
uint8 listOfCommandsToExecuting[NrOfSMSComands];
uint8 counterHowManyCommands;
enum GPRS_commands
{
    SMS=0,
    CALL,
    GPS_STRING,
    totalGPRScomands
};
uint8 GPRSCommands_Counter[totalGPRScomands];
enum GPS_action
{
    deactivate=0,
    activate,
    gps_send_via_sms,
    total_GPS_action
};
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


void init_GPRS_only_on_Startup();
void powerON_GPRS();
void init_GPRS_only_on_PowerOn();
bool wait_gprs_response(uint8 *command,uint8 whaiteTime);
void wait_gprs_loop(void);
bool write_gprs_command(uint8 *command,uint8 *response,uint8 whaiteTime);
void gprs(uint8 action);
#endif /* LIBRARY_GPRS_H_ */
