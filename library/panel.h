/*
 * panel.h
 *
 *  Created on: Mar 27, 2017
 *      Author: uidg6243
 */

#ifndef LIBRARY_PANEL_H_
#define LIBRARY_PANEL_H_
#include <msp430g2955.h>
#include "commontypes.h"
#include "GPRS.h"
#include "key.h"
#define AC BIT0
#define recirculare BIT4
#define parbriz BIT2
#define luneta BIT6
#define ventilator3 BIT0    //port3
#define ventilator4 BIT2    //port3
void activateFromPanel(uint8 activateWhat);
void deactivateFromPanel(uint8 deactivateWhat);
void activateVentilator(uint8 ventilatoorPozition);
void deactivateVentilator(uint8 ventilatoorPozition);
void initPanel();

enum ButtonPanelStates
{
    buttonOff,
    buttonPress,
    buttonRelease
};
uint8 waitPanelTimer;
uint8 buttonState;
uint8 statesOfPannel;
uint8 statesOfVentilator;
#endif /* LIBRARY_PANEL_H_ */
