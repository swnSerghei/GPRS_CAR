/*
 * parkingLight.h
 *
 *  Created on: Mar 27, 2017
 *      Author: uidg6243
 */

#ifndef LIBRARY_PARKINGLIGHT_H_
#define LIBRARY_PARKINGLIGHT_H_
#include <msp430g2955.h>
#include "library/commontypes.h"
#include "GPRS.h"

#define parkingLightPin BIT7
#define StayActive_ParckingLight 360    //6h - 6 * 60
uint16 counter_StayActive_ParckingLight;

void parkingLightInit();
void activateparkingLight();
void deactivateparkingLight();

#endif /* LIBRARY_PARKINGLIGHT_H_ */
