/*
 * interrupts.c
 *
 *  Created on: 05 џэт. 2016 у.
 *      Author: Serghei
 */
#include "interrupts.h"

#pragma vector=TIMER1_A0_VECTOR	//timer
__interrupt void Timer_A1 (void)
{
    static uint8 tmpCounter=0;
    tmpCounter++;
    whaitTimer++;
    waitKeyTimer++;
    tmpCounterForCalculateKeyTime++;
    if ( tmpCounter == 10 )
    {
        tmpCounter = 0;
        seconds++;
        whaitTimer++;

        if (seconds == 60)
        {
            seconds=0;minuts++;
        }
        if (minuts == 60)
        {
            minuts=0;hours++;
        }
        wakeupTimer++;
        if ( wakeupTimer == WAKEUP_AFTER_X_SECONDS && systemState == sleepMode )
        {
            systemState = wakeUpByTimerState;
            wakeUp();
            LPM1_EXIT;
        }
        else if ( wakeupTimer == (WAKEUP_AFTER_X_SECONDS + STAY_AWAKE) && systemState == wakeUpByTimerState )
        {
            systemState = sleepMode;
            //go_to_sleep();
        }
    }
    //if ( (P2IN & KEY2_DIAG) && interruptByKey1_treated )  {systemState = wakeUpByKey2State; interruptByKey1_treated = false;}

    //ms100_ocured = true;
//	LPM1_EXIT;
 	__enable_interrupt();
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void)
{
	if (P1IFG & KEY2_DIAG)
	{
	    P1IFG &= ~KEY2_DIAG;
	    if ( systemState == wakeUpByTimerState ){ interruptByKey1_treated = false; }
	    else wakeUp();
        systemState = wakeUpByKey2State;
        tmpCounterForCalculateKeyTime = 0;
	}
	if (P1IFG & KEY3_DIAG)
    {
        P1IFG &= ~KEY3_DIAG;
        if (carLearn == NotLearn && (tmpCounterForCalculateKeyTime < OVERFLOW_TIME_BETWEEN_KEYS) )
        {
            carLearnTimes++;
            if (carLearnTimes <= LEARNING_TIMES)
            {
                timeBetween2Key_3Key+=tmpCounterForCalculateKeyTime;
                timeBetween2Key_3Key/=2;
                carLearnTimes++;
            }
            else carLearn = Learn;
        }
    }
    __enable_interrupt();
}

#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void)
{
	__enable_interrupt();
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
	if (IFG2 & UCA0TXIFG) {sendNextUartBufferElement_fromInterrupt();IFG2 &= ~UCA0TXIFG;}
	//if (IFG2 & UCA0RXIFG) {Uart_RX_Interrupt();IFG2 &= ~UCA0RXIFG;}
	__enable_interrupt();//__bis_SR_register(GIE);        // interrupts
}
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	if (IFG2 & UCA0RXIFG) {Uart_RX_Interrupt();IFG2 &= ~UCA0RXIFG;}
	__bis_SR_register(GIE);        // interrupts
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	__enable_interrupt();
}
