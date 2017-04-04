/*
 * key.c
 *
 *  Created on: Mar 21, 2017
 *      Author: uidg6243
 */

#include "key.h"

void keyPosition_init()
{
    P1DIR |=   KEY1_ENABLE + KEY2_ENABLE + KEY3_ENABLE;
    P1IES &= ~(KEY2_DIAG + KEY3_DIAG);
    P1IFG &= ~(KEY2_DIAG + KEY3_DIAG);
    P1IE   =   KEY2_DIAG + KEY3_DIAG;
    carLearn = NotLearn;
    carLearnTimes = 0;
    timeBetween2Key_3Key = 0;
    holdKey3Time = 0;
    //timeBetween2Key_3Key = 0;//
    KarStastes = KeysOff;
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
void reportMethod()
{
       write_gprs_command("AT+CMGS=\"0757294327\"\r\n",">\r\n",10);
       print("Learn:%d\r\n",carLearn);
       print("rotation:%d\r\n",avgRotationSpeed);
       print("time_2Key_3Key:%d\r\n",timeBetween2Key_3Key);
       print("holdKey3Time:%d\r\n",holdKey3Time);
       print("parkLight:%d\r\n",parkingLightState);
       print("KarStastes:%d\r\n",KarStastes);
       print("parkBreak:%d\r\n",parkingState());
       print("shifter:%d\r\n",shifterState());
       print("ventilator:%d\r\n",statesOfVentilator);
       print("panel:%d\r\n",statesOfPannel);
       putInUartBuffer(26);//ctrl+z
       wait_gprs_response("OK",10);
       counterExecutedCommands++;
}
void startEngine()
{
    if ( carLearn == Learn )
    {
        if ( (retryersToStartEngine < RETRY_TIMES) && parkingState() && shifterState() )
        {
                if ( KarStastes == KeysOffFailed && waitKeyTimer >= 10)
               {
                   P1OUT |= KEY1_ENABLE; KarStastes = Key1on;waitKeyTimer=0;
#ifdef debugMode == 1
                   print("activate key1 from KeysOffFail\r\n");
#endif
               }
               else if ( KarStastes == KeysOff )
               {
                   P1OUT |= KEY1_ENABLE; KarStastes = Key1on;waitKeyTimer=0;

#ifdef debugMode == 1
                   print("activate key1 from KeysOff\r\n");
#endif
               }
               else if ( KarStastes == Key1on && waitKeyTimer >= 10 )
               {
                   P1OUT |= KEY2_ENABLE; KarStastes = Key2on;waitKeyTimer=0;
#ifdef debugMode == 1
                   print("activate key2\r\n");
#endif
               }
               else if ( KarStastes == Key2on && waitKeyTimer >= timeBetween2Key_3Key)
               {
#ifdef debugMode == 1
                   print("activate key3\r\n");
#endif
                   OverRotationTimeValidation = OVER_ROTATION_TIME;avgRotationSpeedValidate = 0;
                   P1OUT |= KEY3_ENABLE;KarStastes = Key3on; waitKeyTimer =0;
                   TA0CTL |= MC_2;TA0IV &= ~TA0IV_TAIFG;TA0CTL |= TAIE ;
               }
               else if ( KarStastes == Key3on && waitKeyTimer >= (holdKey3Time + HOLD_KEY3_MORE))
               {
                   P1OUT &= ~KEY3_ENABLE;
                   KarStastes = Key3fail1;waitKeyTimer =0;
                   retryersToStartEngine++;
#ifdef debugMode == 1
                   print("engine start fail %d,with rotation %d\r\n",retryersToStartEngine,avgRotationSpeedValidate);
#endif
               }
               else if ( KarStastes == motorStarted )
               {
                   counterExecutedCommands++; counter_StayActive_EngineStart = 0;
                   OverflowOcuredInEngineStarted = 0;P1OUT &= ~KEY3_ENABLE;
#ifdef debugMode == 1
                   print("engine started:%d\r\n",avgRotationSpeedValidate);
#endif
               }
               else if ( KarStastes == Key3fail1 && waitKeyTimer >= 10 )
               {
                   P1OUT &= ~KEY2_ENABLE;
                   KarStastes = key3fail2;waitKeyTimer =0;
#ifdef debugMode == 1
                   print("Key3fail1\r\n");
#endif
               }
               else if ( KarStastes == key3fail2 && waitKeyTimer >= 10 )
               {
                 P1OUT &= ~KEY1_ENABLE;
                 KarStastes = KeysOffFailed; waitKeyTimer = 0;
#ifdef debugMode == 1
                 print("Key3fail2\r\n");
#endif
               }
        }
        else
        {
            //waitKeyTimer = 0;
            listOfCommandsToExecuting[counterExecutedCommands] = engine_stop;
//            if ( KarStastes == Key3on ) { P1OUT &= ~KEY3_ENABLE; waitKeyTimer = 0; KarStastes = Key2on; listOfCommandsToExecuting[counterExecutedCommands] = engine_stop; }
//            else if ( KarStastes == Key2on || KarStastes == Key3fail1 ) { P1OUT &= ~KEY2_ENABLE; waitKeyTimer = 0; KarStastes = Key1on; listOfCommandsToExecuting[counterExecutedCommands] = engine_stop; }
//            else { P1OUT &= ~( KEY3_ENABLE + KEY2_ENABLE + KEY1_ENABLE );counterExecutedCommands++; }
#ifdef debugMode == 1
            print("engine can't start, to much retryes, breake or shifter\r\n");
#endif
        }
    }
    else
    {
        counterExecutedCommands++;
#ifdef debugMode == 1
        print("engine not learned\r\n");
#endif
    }
}
void stopEngine()
{
    TA0CTL &= ~(MC_2 + TAIE);P2IE &= ~ROTATION_PIN;
    if ( KarStastes == Key3on ) { KarStastes = Key2on; P1OUT &= ~KEY3_ENABLE; waitKeyTimer = 0; }
    else if ( (KarStastes == motorStarted || KarStastes == Key2on || KarStastes == Key3fail1 ) && waitKeyTimer >= 10 ) { KarStastes = Key1on;P1OUT &= ~KEY2_ENABLE;waitKeyTimer = 0;}
    else if ( ((KarStastes == Key1on || KarStastes == key3fail2) &&  waitKeyTimer >= 10) || KarStastes == KeysOff ) { KarStastes = KeysOff;P1OUT &= ~KEY1_ENABLE; counterExecutedCommands++;}
    //else { P1OUT &= ~( KEY3_ENABLE + KEY2_ENABLE + KEY1_ENABLE ); KarStastes = KeysOff; counterExecutedCommands++; }//to be sure all keys off
}
void activateKey1()
{
    if ( KarStastes == KeysOff )
    {
        P1OUT |=  KEY1_ENABLE;KarStastes = Key1on;
#ifdef debugMode == 1
        print("key1_start\r\n");
#endif
    }
    counterExecutedCommands++;
}
void deactivateKey1()
{
    if ( KarStastes == Key1on )
    {
        P1OUT &= ~KEY1_ENABLE;KarStastes = KeysOff;
#ifdef debugMode == 1
        print("key1_stoped\r\n");
#endif
    }
    counterExecutedCommands++;
}
