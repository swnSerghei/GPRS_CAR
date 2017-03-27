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
/*
 * main.c
 */
void portsOFF();
void main(void)
{
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
    gprs_state_machine = GPRS_INIT;// for allow to go in sleep first time only
    while(1)
    {
        if ( (systemState == sleepMode ) && (!PresentAnyCommand) && (gprs_state_machine != GPRS_SLEEP) && interruptByKey1_treated)
        {
            write_gprs_command("AT+CPOF\r\n","OK\r\n",20);
            while (TxBuffer_Uart_Head != TxBuffer_Uart_Tail){}
            go_to_sleep();
        }
        else
        {
            if ( gprs_state_machine == GPRS_SLEEP ) powerON_GPRS();
            //if ( gprs_state_machine == GPRS_INIT )  wait_gprs_loop();
            wait_gprs_loop();
        }

        if ( counterExecutedCommands < counterHowManyCommands)
        {
            if ( systemState != wakeUpByKey2State && interruptByKey1_treated )
            {
                switch (listOfCommandsToExecuting[counterExecutedCommands])
                {
                case parkigLight_start:    { activateparkingLight();     break; }
                case parkigLight_stop:     { deactivateparkingLight();   break; }
                case key1_start:           { activateKey1();             break; }
                case key1_stop:            { deactivateKey1();           break; }
                case engine_start:         { startEngine();              break; }
                case engine_stop:          { stopEngine();               break; }
                case recirculare_start:    { activate_recirculare();     break; }
                case recirculare_stop:     { deactivate_recirculare();   break; }
                case ac_start:             { activate_AC();              break; }
                case ac_stop:              { deactivate_AC();            break; }
                case luneta_start:         { activate_luneta();          break; }
                case luneta_stop:          { deactivate_luneta();        break; }
                case parbriz_start:        { activate_parbriz();         break; }
                case pabriz_stop:          { deactivate_parbriz();       break; }
                case ventilator_3pozitie:  { ventilator3poz();           break; }
                case ventilator_4pozitie:  { ventilator4poz();           break; }
                case ventilator_off:       { ventilatorOff();            break; }
                default:break;
                }
            }
            else
            {
                if ( !interruptByKey1_treated )
                {
                    if ( StateOfCommands & ((uint32)1<<parkigLight_start) )                                {deactivateparkingLight();    counterExecutedCommands--;}
                    else if ( StateOfCommands & ((uint32)3<<(ventilator_3pozitie+ventilator_4pozitie)) )   {ventilatorOff();             counterExecutedCommands--;}
                    else if ( (uint32)1<<parkigLight_start)                                                {stopEngine();                counterExecutedCommands--;}
                    else interruptByKey1_treated = true;
                }
                //all commands ho can execute in wakeUp by key2
                counterExecutedCommands++;
            }
            if ( counterExecutedCommands == counterHowManyCommands) {counterExecutedCommands = 0; counterHowManyCommands = 0;PresentAnyCommand = false;}
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
