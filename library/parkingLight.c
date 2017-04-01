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
    if ( !(StateOfCommands & ((uint32)1<<parkigLight_start)))
    {
        P4OUT |= parkingLightPin;
        counter_StayActive_ParckingLight = 0;
        StateOfCommands |= ((uint32)1<<parkigLight_start);

        //print("parkigLight_start\r\n");
    }
    else counterExecutedCommands++;
}
void deactivateparkingLight()
{
    if (StateOfCommands & ((uint32)1<<parkigLight_start))
    {
        P4OUT &= ~parkingLightPin;
        StateOfCommands &= ~((uint32)1<<parkigLight_start);
        counterExecutedCommands++;
        //print("parkigLight_stop\r\n");
    }
    else counterExecutedCommands++;
}
