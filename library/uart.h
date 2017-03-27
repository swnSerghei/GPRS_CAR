/*
 * uart.h
 *
 *  Created on: 15.12.2015
 *      Author: uidg6243
 */

#ifndef UART_H_
#define UART_H_
#include <msp430.h>
#include "commontypes.h"

#define RXD BIT5
#define TXD BIT4

#define SizeOfBuffer 255
uint8 TxBuffer_Uart[SizeOfBuffer];
uint16 TxBuffer_Uart_Head;
uint16 TxBuffer_Uart_Tail;
uint8 RxBuffer_Uart[SizeOfBuffer];
uint16 RxBuffer_Uart_Head;
uint16 RxBuffer_Uart_Tail;

void Init_Uart();
void uninit_uart();
void Uart_RX_Interrupt();
void putInUartBuffer(uint8 elementToPutInBuffer);
void sendUartBuffer();
void sendNextUartBufferElement_fromInterrupt();
#endif /* UART_H_ */
