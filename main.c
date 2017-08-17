#include <msp430g2955.h>
#include "library/printf.h"
#include "library/uart.h"
#include "library/interrupts.h"
#include "library/commontypes.h"
#include "library/system.h"
#include "library/key.h"
#include "library/GPRS.h"
#include "library/parkingLight.h"
#include "library/key.h"
#include "library/panel.h"
#define debugMode 0
/*
 * main.c
 */
void portsOFF();
void main(void)
{
    uint8 tmpValue;
    portsOFF();
    watchDogConfigure();
    configureFrequency();
    Init_Uart();
    initTimerOnlyOnPowerOn();
    init_GPRS_only_on_PowerOn();
    keyPosition_init();
    initPanel();
    parkingLightInit();
    __enable_interrupt();
    go_to_sleep();

    {
        if ( (systemState == sleepMode) && (gprs_state_machine == GPRS_INIT) )
        {
            tmpValue = RxBuffer_Uart_Head;
            if ( write_gprs_command("AT+CPOF\r\n","OK",10) || tmpValue == RxBuffer_Uart_Head )
            {
#if debugMode == 1
                if ( tmpValue == RxBuffer_Uart_Head )
                {
                    print("not came any response\r\n");
                    while ( TxBuffer_Uart_Head != TxBuffer_Uart_Tail ) {}
                }
                else
                {
                    print("positive response\r\n");
                    while ( TxBuffer_Uart_Head != TxBuffer_Uart_Tail ) {}
                }
#endif
                go_to_sleep();//if not came any response or maybe in exact this time came any message
            }
            else
            {
                RxBuffer_Uart_Head =  tmpValue; systemState = wakeUpByTimerState;
#if debugMode == 1
                print("tmpValue |= RxBuffer_Uart_Head and negative response on AT+CPOF, maybe came message\r\n");
#endif
            }
        }

        if ( gprs_state_machine == GPRS_SLEEP ) powerON_GPRS();

        wait_gprs_loop();

        if ( counterExecutedCommands < counterHowManyCommands)
        {
            if ( systemState == wakeUpByTimerState )
            {
                switch (listOfCommandsToExecuting[counterExecutedCommands])
                {
                case parkingLight_start:   { activateparkingLight();                            break; }
                case parkingLight_stop:    { deactivateparkingLight();                          break; }
                case key1_start:           { activateKey1();                                    break; }
                case key1_stop:            { deactivateKey1();                                  break; }
                case engine_start:         { startEngine();                                     break; }
                case engine_stop:          { stopEngine();                                      break; }
                case recirculare_start:    { activateFromPanel(recirculare);                    break; }
                case recirculare_stop:     { deactivateFromPanel(recirculare);                  break; }
                case ac_start:             { activateFromPanel(AC);                             break; }
                case ac_stop:              { deactivateFromPanel(AC);                           break; }
                case luneta_start:         { activateFromPanel(luneta);                         break; }
                case luneta_stop:          { deactivateFromPanel(luneta);                       break; }
                case parbriz_start:        { activateFromPanel(parbriz);                        break; }
                case pabriz_stop:          { deactivateFromPanel(parbriz);                      break; }
                case ventilator_3pozitie:  { activateVentilator(ventilator3);                   break; }
                case ventilator_4pozitie:  { activateVentilator(ventilator4);                   break; }
                case ventilator_off:       { deactivateVentilator(ventilator3 + ventilator4);   break; }
                case report:               { reportMethod();                                    break; }
                case gps_on:               { gprs(activate);                                    break; }
                case gps_off:              { gprs(deactivate);                                  break; }
                case gps_get:              { gprs(gps_send_via_sms);                            break; }
                case unlock_doors:         { unlock();                                          break; }
                case delaySeconds:         { }
                default:break;
                }
            }
            else if (systemState == wakeUpByKey2State)
            {
                //commands in wakeupBy2Key
                switch (listOfCommandsToExecuting[counterExecutedCommands])
                {
                case report:               { reportMethod();                                    break; }
                case gps_on:               { gprs(activate);                                    break; }
                case gps_off:              { gprs(deactivate);                                  break; }
                case gps_send_via_sms:     { gprs(gps_send_via_sms);                            break; }
                default:                   { counterExecutedCommands++;                         break; }
                }
            }
        }
        else
        {
            counterExecutedCommands = 0; counterHowManyCommands = 0; allCommandsExecuted = true;
        }
    }
}
void portsOFF()
{
    P1OUT = 0;
    P1DIR = 0;
    P1SEL = 0;
    P1SEL2= 0;
    P2OUT = 0;
    P2DIR = 0;
    P2SEL = 0;
    P2SEL2= 0;
    P3OUT = 0;
    P3DIR = 0;
    P3SEL = 0;
    P3SEL2= 0;
    P4OUT = 0;
    P4DIR = 0;
    P4SEL = 0;
    P4SEL2= 0;
}
