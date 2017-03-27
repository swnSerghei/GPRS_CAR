/*
 * system.c
 *
 *  Created on: 18 мар. 2017 г.
 *      Author: Serghei
 */
#include "system.h"
void watchDogConfigure()
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    //SMCLK => 8Mhz
    //      Password | timerMode|cleareCounter|   div        div
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL  | WDTIS1  |   WDTIS0  ;   //8uS,
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL  | WDTIS1              ;   //65us
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL            |   WDTIS0  ;   //1.044mS
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL                        ;   //4.176ms

    //SMCLK => 4Mhz
    //      Password | timerMode|cleareCounter|   div        div
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL  | WDTIS1  |   WDTIS0  ;   //16.24-16.29uS
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL  | WDTIS1              ;   //130.1us
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL            |   WDTIS0  ;   //2.082ms
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL                        ;   //8.328ms

    //SMCLK => 2Mhz
    //      Password | timerMode|cleareCounter|   div        div
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL  | WDTIS1  |   WDTIS0  ;   //32.48-32.58uS
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL  | WDTIS1              ;   //260.2us
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL            |   WDTIS0  ;   //4.165ms
    //WDTCTL = WDTPW | WDTTMSEL |   WDTCNTCL                        ;   //16.656ms

    //IE1 |= WDTIE;                         // enable Watchdog timer interrupts
}
void configureFrequency()
{
    DCOCTL = CALDCO_16MHZ;                  // Set DCO step and modulation
    BCSCTL1 = CALBC1_16MHZ;                 // Set range
    BCSCTL2 = DIVS_2;                       //SMCLK set 16Mhz/4=4Mhz
}
#pragma vector=WDT_VECTOR                   // Watchdog Timer interrupt service routine

  __interrupt void watchdog_timer(void)
{
      //WatchDogTask();
      //P3OUT ^= BIT6;
      //_BIC_SR(LPM3_EXIT); // wake up from low power mode
      //IE1 &= ~WDTIE;                         // enable Watchdog timer interrupts
}
