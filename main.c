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
        if ( (systemState == sleepMode ) && (gprs_state_machine == GPRS_INIT) && interruptByKey2_treated)
        {
            write_gprs_command("AT+CPOF\r\n","OK\r\n",20);
            while (TxBuffer_Uart_Head != TxBuffer_Uart_Tail){}
            go_to_sleep();
        }
        else
        {
            if ( gprs_state_machine == GPRS_SLEEP ) powerON_GPRS();
            else wait_gprs_loop();

            //if ( gprs_state_machine == GPRS_INIT )  wait_gprs_loop();

        }
        if ( needToSendSMS && gprs_state_machine == GPRS_INIT)
        {
            write_gprs_command("AT+CMGS=\"0757294327\"\r\n",">\r\n",10);
            print("the car learned:\r\n");
            print("rotaion with:%d\r\n",avgRotationSpeed);
            print("time between Key2 and key3:%dmS\r\n",timeBetween2Key_3Key*100);
            print("time hold key3:%dmS\r\n",holdKey3Time*100);
            putInUartBuffer(26);//ctrl+z
            wait_gprs_response("OK",10);
            needToSendSMS = false;
        }
        if ( !interruptByKey2_treated )
        {
            P4OUT &= ~parkingLightPin;
            P3OUT &= ~( ventilator_3pozitie + ventilator_4pozitie );
            P1OUT &= ~( KEY1_ENABLE+ KEY2_ENABLE  + KEY3_ENABLE ); KarStastes = KeysOff;

            if ( carLearn == Learn ) { TA0CTL &= ~(MC_2 + TAIE);P2IE &= ~ROTATION_PIN; }

            StateOfCommands &= ~((uint32)1<<parkigLight_start);
            StateOfCommands &= ~((uint32)3<<ventilator_3pozitie);
            StateOfCommands &= ~((uint32)1<<key1_start);
            StateOfCommands &= ~((uint32)1<<engine_start);

            StateOfCommands &= ~((uint32)1<<ac_start);
            StateOfCommands &= ~((uint32)1<<luneta_start);
            StateOfCommands &= ~((uint32)1<<parbriz_start);
            StateOfCommands &= ~((uint32)1<<recirculare_start);

            if ( allCommandsExecuted != true ) counterExecutedCommands++;
            interruptByKey2_treated = true;
        }

        if ( counterExecutedCommands < counterHowManyCommands)
        {
            if ( systemState == wakeUpByTimerState )
            {
                if ( interruptByKey2_treated )
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
            }
            else if (systemState == wakeUpByKey2State)
            {
                //commands for wakeupByKey
                counterExecutedCommands++;
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
