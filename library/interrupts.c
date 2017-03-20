/*
 * interrupts.c
 *
 *  Created on: 05 џэт. 2016 у.
 *      Author: Serghei
 */
#include "interrupts.h"

#pragma vector=TIMER1_A1_VECTOR	//timer 1
__interrupt void Timer_A1 (void)
{
	switch(TA1IV)
	{
	 case 2:	// CCR1
	 break;
	 case 4:	// CCR2
	 break;
	 case 10: 	Flag33msOcured = true;// overflow ? reserved
	 break;
	 case 14: 	// overflow ?
	 break;
	 default:	//default :)
	 break;
	}
 	__enable_interrupt();
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void)
{
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
	if (IFG2 & UCA0RXIFG) {Uart_RX_Interrupt();IFG2 &= ~UCA0RXIFG;}
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
