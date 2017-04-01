/*
 * key.c
 *
 *  Created on: Mar 21, 2017
 *      Author: uidg6243
 */

#include "key.h"

void keyPosition_init()
{
    P1DIR |=    KEY1_ENABLE + KEY2_ENABLE + KEY3_ENABLE;
    P1IES &=   ~(KEY2_DIAG + KEY3_DIAG);
    P1IE   =    KEY2_DIAG + KEY3_DIAG;
    //carLearn = NotLearn;
    carLearn = NotLearn;
    carLearnTimes = 0;
    timeBetween2Key_3Key = 0;
    holdKey3Time = 0;
    //timeBetween2Key_3Key = 0;//
    KarStastes = KeysOff;
    interruptByKey2_treated = true;
    counter_StayActive_ParckingLight = 0;
    counter_StayActive_EngineStart = 0;

    P2IFG &= ~ROTATION_PIN;
    avgRotationSpeed = 0;
    OverFlowOcured = 0;
}
bool parkingState()
{
    volatile bool returnTmpValue = P3IN & PARKING_BRAKE;
    if ( returnTmpValue ==  PARKING_BRAKE) returnTmpValue = 0;
    else returnTmpValue = 1;
    return returnTmpValue;
}
bool shifterState()
{
    volatile bool returnTmpValue = P3IN & SHIFTER;
    if (returnTmpValue == SHIFTER) returnTmpValue = 0;
    else returnTmpValue = 1;
    return returnTmpValue;
}
void startEngine()
{
    if ( carLearn == Learn )
    {
        if (! (StateOfCommands & (uint32)1<<engine_start) && retryersToStartEngine < RETRY_TIMES && parkingState() && shifterState() )
        {
                if ( KarStastes == KeysOffFailed && waitKeyTimer >= 10)
                {
                    P1OUT |= KEY1_ENABLE; KarStastes = Key1on;waitKeyTimer=0;
                    //print("activate key1 from KeysOffFail\r\n");
                }
           else if ( KarStastes == KeysOff )
           {
               P1OUT |= KEY1_ENABLE; KarStastes = Key1on;waitKeyTimer=0;
               //print("activate key1 from KeysOff\r\n");
           }
           else if ( KarStastes == Key1on && waitKeyTimer >= 10 || (StateOfCommands & ((uint32)1<<key1_start)) )
           {
               StateOfCommands &= ~((uint32)1<<key1_start);P1OUT |= KEY2_ENABLE; KarStastes = Key2on;waitKeyTimer=0;
               //print("activate key2\r\n");
           }
           else if ( KarStastes == Key2on && waitKeyTimer >= timeBetween2Key_3Key)
           {
               //print("activate key3\r\n");
               OverRotationTimeValidation = 3;
               avgRotationSpeedValidate = 0;
               P1OUT |= KEY3_ENABLE;KarStastes = Key3on; waitKeyTimer =0;
               TA0CTL |= MC_2;
               TA0IV &= ~TA0IV_TAIFG;
               TA0CTL |= TAIE ;
           }
           else if ( KarStastes == Key3on && waitKeyTimer >= (holdKey3Time + HOLD_KEY3_MORE))
           {
               P1OUT &= ~KEY3_ENABLE;KarStastes = Key3fail1;
               waitKeyTimer =0;retryersToStartEngine++;
               //print("engine start fail %d,with rotation %d\r\n",retryersToStartEngine,avgRotationSpeedValidate);
           }
           else if ( KarStastes == motorStart ) {counterExecutedCommands++; counter_StayActive_EngineStart = 0;OverflowOcuredInEngineStarted = 0;P1OUT &= ~KEY3_ENABLE; StateOfCommands |= (uint32)1<<engine_start;
           //print("engine started:%d\r\n",avgRotationSpeedValidate);
           }
           else if ( KarStastes == Key3fail1 && waitKeyTimer >= 10 ) {P1OUT &= ~KEY2_ENABLE;KarStastes = key3fail2;waitKeyTimer =0;
           //print("Key3fail1\r\n");
           }
           else if ( KarStastes == key3fail2 && waitKeyTimer >= 10 )
           {
             P1OUT &= ~KEY1_ENABLE;
             //print("Key3fail2\r\n");
             KarStastes = KeysOffFailed; waitKeyTimer = 0;
           }
        }
        else
        {
            KarStastes = KeysOff;
            P1OUT &= ~( KEY3_ENABLE + KEY2_ENABLE + KEY1_ENABLE );
            counterExecutedCommands++;
            //print("engine can't start, is laready started or to much retryes to start\r\n");
        }
    }
    else
    {
        counterExecutedCommands++;
        //print("engine not learned\r\n");
    }
}
void stopEngine()
{
        if ( StateOfCommands & (uint32)1<<engine_start )
        {
            P1OUT &= ~KEY2_ENABLE;
            StateOfCommands &= ~(uint32)1<<engine_start;
            KarStastes = Key2on;
            waitKeyTimer = 0;
            //print("engine_stop key2\r\n");
        }
   else if ( KarStastes == Key2on && waitKeyTimer >= 10 )
   {
       P1OUT &= ~KEY1_ENABLE;
       KarStastes = KeysOff;
       StateOfCommands &= ~(uint32)1<<key1_start;
       //print("engine_stoped\r\n");
       counterExecutedCommands++;
   }

}
void activateKey1()
{
    if ( KarStastes == KeysOff )
    {
        P1OUT |=  KEY1_ENABLE;KarStastes = Key1on;StateOfCommands |= ((uint32)1<<key1_start);
        //print("key1_start\r\n");
    }
    counterExecutedCommands++;
}
void deactivateKey1()
{

    if ( StateOfCommands & ((uint32)1<<key1_start) )
    {
        P1OUT &= ~KEY1_ENABLE;KarStastes = KeysOff;
        //print("key1_stoped\r\n");
    }
    counterExecutedCommands++;
}
