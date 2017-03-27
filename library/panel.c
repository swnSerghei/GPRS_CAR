/*
 * panel.c
 *
 *  Created on: Mar 27, 2017
 *      Author: uidg6243
 */
#include "panel.h"
void initPanel()
{
 P2DIR |= AC + recirculare + parbriz + luneta;
 P3DIR |= ventilator_3pozitie + ventilator_4pozitie;
 buttonState = buttonOff;
}
void activate_AC()
{
    if ( StateOfCommands & ((uint32)1<<ac_start) ) { counterExecutedCommands++; buttonState = buttonOff;}
    else if (buttonState == buttonOff) {P2OUT |= AC; buttonState = buttonPress; waitPanelTimer=0;print("ac_start\r\n");}
    else if (buttonState == buttonPress && waitPanelTimer > 10){P2OUT &= ~AC; StateOfCommands |= ((uint32)1<<ac_start);counterExecutedCommands++;print("ac_started\r\n");}
}
void deactivate_AC()
{
    if ( StateOfCommands & ~((uint32)1<<ac_start) ) { counterExecutedCommands++; buttonState = buttonOff; }
    else if (buttonState == buttonOff) {P2OUT |= AC; buttonState = buttonPress; waitPanelTimer=0;print("ac_stop\r\n");}
    else if (buttonState == buttonPress && waitPanelTimer > 10){P2OUT &= ~AC; StateOfCommands &= ~((uint32)1<<ac_start);counterExecutedCommands++;print("ac_stoped\r\n");}
}
void activate_recirculare()
{
    if ( StateOfCommands & ~((uint32)1<<recirculare_start) ) { counterExecutedCommands++; buttonState = buttonOff; }
    else if (buttonState == buttonOff) {P2OUT |= recirculare; buttonState = buttonPress; waitPanelTimer=0;print("recirculare_start\r\n");}
    else if (buttonState == buttonPress && waitPanelTimer > 10){P2OUT &= ~recirculare; StateOfCommands |= ((uint32)1<<recirculare_start);counterExecutedCommands++;print("recirculare_started\r\n");}
}
void deactivate_recirculare()
{
    if ( StateOfCommands & ~((uint32)1<<recirculare_start) ) { counterExecutedCommands++; buttonState = buttonOff; }
    else if (buttonState == buttonOff) {P2OUT |= recirculare; buttonState = buttonPress; waitPanelTimer=0;print("recirculare_stop\r\n");}
    else if (buttonState == buttonPress && waitPanelTimer > 10){P2OUT &= ~recirculare; StateOfCommands &= ~((uint32)1<<recirculare_start);counterExecutedCommands++;print("recirculare_stoped\r\n");}
}
void activate_parbriz()
{
    if ( StateOfCommands & ~((uint32)1<<parbriz_start) ) { counterExecutedCommands++; buttonState = buttonOff; }
    else if (buttonState == buttonOff) {P2OUT |= parbriz; buttonState = buttonPress; waitPanelTimer=0;print("parbriz_start\r\n");}
    else if (buttonState == buttonPress && waitPanelTimer > 10){P2OUT &= ~parbriz; StateOfCommands |= ((uint32)1<<parbriz_start);counterExecutedCommands++;print("parbriz_started\r\n");}
}
void deactivate_parbriz()
{
    if ( StateOfCommands & ~((uint32)1<<parbriz_start) ) { counterExecutedCommands++; buttonState = buttonOff; }
    else if (buttonState == buttonOff) {P2OUT |= parbriz; buttonState = buttonPress; waitPanelTimer=0;print("pabriz_stop\r\n");}
    else if (buttonState == buttonPress && waitPanelTimer > 10){P2OUT &= ~parbriz; StateOfCommands &= ~((uint32)1<<parbriz_start);counterExecutedCommands++;print("pabriz_stoped\r\n");}
}
void activate_luneta()
{
    if ( StateOfCommands & ~((uint32)1<<luneta_start) ) { counterExecutedCommands++; buttonState = buttonOff; }
    else if (buttonState == buttonOff) {P2OUT |= luneta; buttonState = buttonPress; waitPanelTimer=0;print("luneta_start\r\n");}
    else if (buttonState == buttonPress && waitPanelTimer > 10){P2OUT &= ~luneta; StateOfCommands |= ((uint32)1<<luneta_start);counterExecutedCommands++;print("luneta_started\r\n");}
}
void deactivate_luneta()
{
    if ( StateOfCommands & ~((uint32)1<<luneta_start) ) { counterExecutedCommands++; buttonState = buttonOff; }
    else if (buttonState == buttonOff) {P2OUT |= luneta; buttonState = buttonPress; waitPanelTimer=0;print("luneta_stop\r\n");}
    else if (buttonState == buttonPress && waitPanelTimer > 10){P2OUT &= ~luneta; StateOfCommands &= ~((uint32)1<<luneta_start);counterExecutedCommands++;print("luneta_stoped\r\n");}
}

void ventilator3poz()
{
    if ( StateOfCommands & ((uint32)1<<ventilator_4pozitie) )
    { P3OUT |= ventilator3;P3OUT &= ~ventilator4; StateOfCommands |= ((uint32)(uint32)1<<ventilator_3pozitie);StateOfCommands &= ~((uint32)1<<ventilator_4pozitie); print("ventilator_3pozitie\r\n");}
    else P3OUT |= ventilator3;
    counterExecutedCommands++;
}
void ventilator4poz()
{
    if ( StateOfCommands & ((uint32)1<<ventilator_3pozitie) )
    { P3OUT |= ventilator4;P3OUT &= ~ventilator3;StateOfCommands |= ((uint32)(uint32)1<<ventilator_4pozitie);StateOfCommands &= ~((uint32)1<<ventilator_3pozitie);print("ventilator_4pozitie\r\n");}
    else P3OUT |= ventilator3;
    counterExecutedCommands++;
}
void ventilatorOff()
{
    P3OUT &= ~(ventilator_3pozitie + ventilator_4pozitie);
    StateOfCommands &= ~((uint32)1<<(ventilator_3pozitie+ventilator_4pozitie));
    print("ventilator_off\r\n");
    counterExecutedCommands++;
}
