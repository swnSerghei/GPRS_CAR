/*
 * uart.c
 *
 *  Created on: 15.12.2015
 *      Author: uidg6243
 */
#include "uart.h"

/*
Clock			Baudrate		UCBRx		UCBRSx		UCBRFx	UCOS16 = 0 (continued)
1,048,576       9600            109         2           0
4,000,000       9600            416(160+1)  6           0
4,000,000       115200          34          6           0
8,000,000       9600            833         2           0
16,000,000      9600            1666        6           0
Clock           Baudrate        UCBRx       UCBRSx      UCBRFx  UCOS16 = 1
1,048,576       9600            6           0           13
4,000,000       9600            26          0           1
8,000,000       9600            52          0           1
16,000,000      9600            104         0           3
*/
void Init_Uart()
{
	P3SEL  |=   RXD + TXD ; // P3.4 = RXD, P3.5=TXD
	UCA0CTL1 |= UCSWRST;                      			// Enable SW reset
	UCA0CTL1 |= UCSSEL_2; // SMCLK
	UCA0BR0 = 34; // 4MHz
	UCA0BR1 = 0;
	UCA0MCTL = UCBRS_6;
    //UCA0CTL0 |= UCPAR+UCPEN;
    UCA0CTL1 &= ~UCSWRST;                               // **Initialize USCI state machine**
	IFG2 &= ~UCA0TXIFG;
	IFG2 &= ~UCA0RXIFG;
	UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	UC0IE |= UCA0TXIE; // Enable USCI_A0 RX interrupt

	TxBuffer_Uart_Head = 0;
    TxBuffer_Uart_Tail = 0;
    RxBuffer_Uart_Tail = 0;
    RxBuffer_Uart_Head = 0;
}
void uninit_uart()
{
    P3SEL   &= ~( RXD + TXD ) ; // P3.4 = RXD, P3.5=TXD
//    IFG2    &= ~UCA0TXIFG;
//    IFG2    &= ~UCA0RXIFG;
    UC0IE   &= ~UCA0RXIE; // Enable USCI_A0 RX interrupt
    UC0IE   &= ~UCA0TXIE; // Enable USCI_A0 RX interrupt
}

void putInUartBuffer(uint8 elementToPutInBuffer)
{
    UC0IE   &= ~UCA0TXIE; // Enable USCI_A0 RX interrupt
    if ( TxBuffer_Uart_Head == TxBuffer_Uart_Tail )
    {
        UCA0TXBUF = elementToPutInBuffer;  //send next byte
    }
    else
    {
        TxBuffer_Uart[TxBuffer_Uart_Tail] = elementToPutInBuffer;
    }
    TxBuffer_Uart_Tail++;
    if (TxBuffer_Uart_Tail == SizeOfBuffer ) TxBuffer_Uart_Tail = 0;
	//else if (TxBuffer_Uart_Tail == TxBuffer_Uart_Head) {}//overflow
    UC0IE |= UCA0TXIE; // Enable USCI_A0 RX interrupt
}
void Uart_RX_Interrupt()
{
    RxBuffer_Uart[RxBuffer_Uart_Tail] = UCA0RXBUF;
    RxBuffer_Uart_Tail++;
	if ( RxBuffer_Uart_Tail == SizeOfBuffer ) RxBuffer_Uart_Tail=0;
}
void sendNextUartBufferElement_fromInterrupt()
{
    TxBuffer_Uart_Head++;
    if (TxBuffer_Uart_Head == SizeOfBuffer ) TxBuffer_Uart_Head = 0;
    if ( TxBuffer_Uart_Head != TxBuffer_Uart_Tail) // TX over?
	{
		UCA0TXBUF = TxBuffer_Uart[TxBuffer_Uart_Head];	//send next byte
	}
    else {TxBuffer_Uart_Head = 0;TxBuffer_Uart_Tail = 0;}
}
