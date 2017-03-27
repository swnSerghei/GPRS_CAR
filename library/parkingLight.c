/*
 * parkingLight.c
 *
 *  Created on: Mar 27, 2017
 *      Author: uidg6243
 */
#include "parkingLight.h"
void parkingLightInit()
{
    P4DIR |= parkingLightPin;
}
void activateparkingLight()
{
    P4OUT |= parkingLightPin;
    StateOfCommands |= ((uint32)1<<parkigLight_start);
    counterExecutedCommands++;
    print("parkigLight_start\r\n");
}
void deactivateparkingLight()
{
    P4OUT &= ~parkingLightPin;
    StateOfCommands &= ~((uint32)1<<parkigLight_start);
    counterExecutedCommands++;
    print("parkigLight_stop\r\n");
}
