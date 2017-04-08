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
    parkingLightState = false;
}
void activateparkingLight()
{
    if ( parkingLightState ) counterExecutedCommands++;
    else
    {
#if debugMode == 1
                print("Activate Parking Light\r\n");
#endif
        P4OUT |= parkingLightPin;
        counter_StayActive_ParckingLight = 0;
        parkingLightState |= true;
        //print("parkingLight_start\r\n");
    }
}
void deactivateparkingLight()
{
        P4OUT &= ~parkingLightPin;
        parkingLightState = false;
        counterExecutedCommands++;
#if debugMode == 1
                print("Deactivate Parking Light\r\n");
#endif
        //print("parkingLight_stop\r\n");
}
