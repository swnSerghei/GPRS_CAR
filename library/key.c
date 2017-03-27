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
    P1IE   =    KEY2_DIAG + KEY3_DIAG;
    //carLearn = NotLearn;
    carLearn = Learn;
    carLearnTimes = 0;
    timeBetween2Key_3Key = 50;
    //timeBetween2Key_3Key = 0;//
    KarStastes = KeysOff;
    interruptByKey1_treated = true;
}
void startEngine()
{
    if ( carLearn == Learn)
    {
            if ( KarStastes == KeysOffFailed && waitKeyTimer >= 10) { P1OUT |= KEY1_ENABLE; KarStastes = Key1on;waitKeyTimer=0;print("activate key1 from KeysOffFail\r\n"); }
       else if ( KarStastes == KeysOff ) { P1OUT |= KEY1_ENABLE; KarStastes = Key1on;waitKeyTimer=0; print("activate key1 from KeysOff\r\n");}
       else if ( KarStastes == Key1on && waitKeyTimer >= 10 || (StateOfCommands & ((uint32)1<<key1_start)) ) {print("activate key2\r\n");P1OUT |= KEY2_ENABLE; KarStastes = Key2on;waitKeyTimer=0;}
       else if ( KarStastes == Key2on && waitKeyTimer >= timeBetween2Key_3Key) {print("activate key3\r\n");P1OUT |= KEY3_ENABLE;KarStastes = Key3on; waitKeyTimer =0;}
       else if ( KarStastes == Key3on && waitKeyTimer >= (timeBetween2Key_3Key*2)) {P1OUT &= ~KEY3_ENABLE;KarStastes = Key3fail1;print("engine start fail %d\r\n",retryersToStartEngine);waitKeyTimer =0;retryersToStartEngine++;}
       else if ( KarStastes == motorStart ) {counterExecutedCommands++; StateOfCommands |= (uint32)1<<key1_start;print("engine started\r\n");}
       else if ( KarStastes == Key3fail1 && waitKeyTimer >= 10 ) {P1OUT &= ~KEY2_ENABLE;KarStastes = key3fail2;waitKeyTimer =0;}
       else if ( KarStastes == key3fail2 && waitKeyTimer >= 10 )
       {
         P1OUT &= ~KEY1_ENABLE;
         if (retryersToStartEngine < RETRY_TIMES) { KarStastes = KeysOffFailed; waitKeyTimer = 0;}
         else { KarStastes = KeysOff; retryersToStartEngine = 0;counterExecutedCommands++;}
       }
    }
    else counterExecutedCommands++;
}
void stopEngine()
{
        if ( StateOfCommands & (uint32)1<<key1_start ) { P1OUT &= ~KEY2_ENABLE; KarStastes = Key2on; waitKeyTimer = 0; print("engine_stop\r\n");}
   else if ( KarStastes == Key2on && waitKeyTimer >= 10 ) { P1OUT &= ~KEY1_ENABLE; KarStastes = KeysOff; StateOfCommands &= ~(uint32)1<<key1_start; print("engine_stoped\r\n");}
}
void activateKey1()
{
    print("key1_start\r\n");
    if ( KarStastes == KeysOff ) {P1OUT |=  KEY1_ENABLE;KarStastes = Key1on;StateOfCommands |= ((uint32)1<<key1_start);}
}
void deactivateKey1()
{
    print("key1_stop\r\n");
    if ( StateOfCommands & ((uint32)1<<key1_start) ) {P1OUT &= ~KEY1_ENABLE;KarStastes = KeysOff;print("key1_stoped\r\n");}
}
