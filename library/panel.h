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
#define AC BIT0
#define recirculare BIT4
#define parbriz BIT2
#define luneta BIT6
#define ventilator3 BIT0    //port3
#define ventilator4 BIT2    //port3
void activate_AC();
void activate_recirculare();
void activate_parbriz();
void activate_luneta();
void deactivate_AC();
void deactivate_recirculare();
void deactivate_parbriz();
void deactivate_luneta();
void ventilatorOff();
void ventilator4poz();
void ventilator3poz();
void initPanel();
uint8 waitPanelTimer;
enum PanelStates
{
    buttonOff,
    buttonPress,
    buttonRelease
};
uint8 buttonState;
#endif /* LIBRARY_PANEL_H_ */
