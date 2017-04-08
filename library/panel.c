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
 P3DIR |= ventilator3 + ventilator4;
 statesOfPannel = 0;
 statesOfVentilator = 0;
 buttonState = buttonOff;
}
void activateFromPanel(uint8 activateWhat)
{
    if ( KarStastes == motorStarted )
    {
        switch ( activateWhat )
        {
            case AC:
            {
                if ( statesOfVentilator & (ventilator3 + ventilator4) )
                {
                    if ( statesOfPannel & AC ) { counterExecutedCommands++; buttonState = buttonOff;}
                    else if (buttonState == buttonOff) {P2OUT |= AC; buttonState = buttonPress; waitPanelTimer=0;}
                    else if ( waitPanelTimer > 5){P2OUT &= ~AC; buttonState = buttonOff; statesOfPannel |= AC;counterExecutedCommands++;}
                }
                else counterExecutedCommands++;//can't execute because ventilators is off
                break;
            }
            default:
            {
                if ( statesOfPannel & activateWhat ) { counterExecutedCommands++; buttonState = buttonOff;}
                else if (buttonState == buttonOff) {P2OUT |= activateWhat; buttonState = buttonPress; waitPanelTimer=0;}
                else if ( waitPanelTimer > 5){P2OUT &= ~activateWhat; buttonState = buttonOff; statesOfPannel |= activateWhat;counterExecutedCommands++;}
                break;
            }
        }
    }
    else counterExecutedCommands++;//engine not start
}
void deactivateFromPanel(uint8 deactivateWhat)
{
    if ( KarStastes == motorStarted )
    {
        if ( !(statesOfPannel & deactivateWhat) ) { counterExecutedCommands++; buttonState = buttonOff;}
        else if (buttonState == buttonOff) {P2OUT |= deactivateWhat; buttonState = buttonPress; waitPanelTimer=0;}
        else if ( waitPanelTimer > 5){P2OUT &= ~deactivateWhat; buttonState = buttonOff; statesOfPannel &= ~deactivateWhat;counterExecutedCommands++;}
    }
    else //engine not start
    {
        statesOfPannel &= ~deactivateWhat; counterExecutedCommands++;
    }
}
void activateVentilator(uint8 ventilatoorPozition)
{
    if ( KarStastes == motorStarted )
    {
        if ( statesOfVentilator & ventilatoorPozition ) counterExecutedCommands++;
        else
        {
#if debugMode == 1
                print("Activate ventilator\r\n");
#endif
            P3OUT |= ventilatoorPozition;
            statesOfVentilator |= ventilatoorPozition;
            P3OUT &= ~(ventilator4+ventilator3-ventilatoorPozition);
            statesOfVentilator &= ~(ventilator4+ventilator3-ventilatoorPozition);
            counterExecutedCommands++;
        }
    }
    else counterExecutedCommands++;
}
void deactivateVentilator(uint8 ventilatoorPozition)
{
#if debugMode == 1
                print("Dectivate ventilator\r\n");
#endif
    P3OUT &= ~ventilatoorPozition;
    statesOfVentilator &= ~ventilatoorPozition;
    counterExecutedCommands++;
}
