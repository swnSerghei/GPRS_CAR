/*
 * task.c
 *
 *  Created on: 20 мар. 2017 г.
 *      Author: Serghei
 */
#include "task.h"

void task1s()
{
}
void task100ms()
{

}
void go_to_sleep()
{
    TxBuffer_Uart_Head = 0;
    TxBuffer_Uart_Tail = 0;
    gprs_second = 0;
    gprs_minuts = 0;
    wakeupTimer = 0;
    interruptByKey1_treated = true;
    gprs_state_machine = GPRS_SLEEP;
    uninit_uart();
    LPM1; // Enter LPM1 w/interrupt
}
void wakeUp()
{
    PresentAnyCommand = 0;
    retryersToStartEngine = 0;
    Init_Uart();
    // exits LPM3
}
