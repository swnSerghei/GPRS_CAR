/*
 * uart.c
 *
 *  Created on: 15.12.2015
 *      Author: uidg6243
 */
#include "uart.h"
#include "printf.h"
/*
Clock			Baudrate		UCBRx		UCBRSx		UCBRFx	UCOS16 = 0 (continued)
1,048,576       9600            109         2           0
4,000,000       9600            416         6           0
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
	P3SEL2 &= !(RXD + TXD) ;
	UCA0CTL1 |= UCSWRST;                      			// Enable SW reset
	UCA0CTL1 |= UCSSEL_2; // SMCLK
	UCA0BR0 = 160; // 4MHz
	UCA0BR1 = 1;
	UCA0MCTL = UCBRS_6;
	UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
	IFG2 &= ~UCA0TXIFG;
	IFG2 &= ~UCA0RXIFG;
	UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	UC0IE |= UCA0TXIE; // Enable USCI_A0 RX interrupt
	TxBuffer_Uart_Head = 0;
	TxBuffer_Uart_Tail = 0;
	sendInProgress = false;
}
void Uart_RecalculateTail_Buffer(uint8 elementToPutInBuffer)
{
    TxBuffer_Uart[TxBuffer_Uart_Tail] = elementToPutInBuffer;
    TxBuffer_Uart_Tail++;
	if (TxBuffer_Uart_Tail == SizeOfBuffer ) TxBuffer_Uart_Tail=0;
	else if (TxBuffer_Uart_Tail == TxBuffer_Uart_Head) {}//overflow
}
void Uart_command(uint8 NrOfElements)
{

}
void sendUartBuffer()
{
    if ( !sendInProgress )
    {
        sendInProgress = true;
        UCA0TXBUF = TxBuffer_Uart[TxBuffer_Uart_Head];  //send next byte
        if (TxBuffer_Uart_Head == (SizeOfBuffer-1) ) TxBuffer_Uart_Head = 0;
        else TxBuffer_Uart_Head++;
    }
}
void Uart_RX_Interrupt(void)
{
	static uint16 count=0;

//	if ( count == SizeOfBuffer )
//	{
//		for (i=0; i < (SizeOfBuffer-1);i++)//need to test
//		{
//			RxBuffer_Uart[i] = RxBuffer_Uart[i++];
//		}
//		RxBuffer_Uart[count-1] = UCA0RXBUF;
//		if (RxBuffer_Uart[count-1] == 10)	//new line
//		{
//			Uart_command(count);
//			count=0;
//		}
//	}
//	else
		{
		RxBuffer_Uart[count] = UCA0RXBUF;
		if (RxBuffer_Uart[count] == 10)	//new line
		{
			//Uart_command(count);
		    putInUartBuffer("%c",RxBuffer_Uart[count]);
		    sendUartBuffer();
		    count=0;
		}
		else
        {
		    putInUartBuffer("%c",RxBuffer_Uart[count]);
		    count++;
        }
		}
}
void sendNextUartBufferElement_fromInterrupt(void)
{
	if ( TxBuffer_Uart_Head != TxBuffer_Uart_Tail) // TX over?
	{
		UCA0TXBUF = TxBuffer_Uart[TxBuffer_Uart_Head];	//send next byte
		if (TxBuffer_Uart_Head == (SizeOfBuffer-1) ) TxBuffer_Uart_Head = 0;
		else TxBuffer_Uart_Head++;
	}
	else {sendInProgress = false;}
}
