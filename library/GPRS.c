/*
 * GPRS.c
 *
 *  Created on: Mar 21, 2017
 *      Author: uidg6243
 */
#include "GPRS.h"
const uint8 *SMSCommands[]=
{
    {"parkigLight start"},
    {"parkigLight stop"},
    {"key1 start"},
    {"key1 stop"},
    {"engine start"},
    {"engine stop"},
    {"recirculare start"},
    {"recirculare stop"},
    {"ac start"},
    {"ac stop"},
    {"luneta start"},
    {"luneta stop"},
    {"parbriz start"},
    {"pabriz stop"},
    {"ventilator 3pozitie"},
    {"ventilator 4pozitie"},
    {"ventilator off"},
    {"delaySeconds"}
};

const uint8 *GPRSCommands[]=
{
    {"+CMT: \"+"},
    {"call"}
};


uint8 *telephonNumbers[]=
{
 {"40757294327"},
 {"37379548801"}
};


void init_GPRS_only_on_PowerOn()
{
    uint8 i;
    P4DIR |= PWR_GPRS;
    PresentAnyCommand = false;
    counterHowManyCommands=0;
    counterExecutedCommands=0;
    for (i=0;i<NrOfSMSComands;i++)
    {
        listOfCommandsToExecuting[i] = 0;
    }
}
void powerON_GPRS()
{
    print("Activate pin: %d, %d, %d \r\n",seconds,minuts,hours);
    P4OUT |= PWR_GPRS;
    whaitTimer=0;while (whaitTimer < POWER_ON_BUTTON){}
    P4OUT &= ~PWR_GPRS;
    wait_gprs_response("CREG",60);                     //whaite CREG
    wait_gprs_response("CREG",60);                     //whaite CREG
    wait_gprs_response("bla",10);                                             //only for whaite 1 seconds
    write_gprs_command("AT+CPIN=\"1989\"\r\n","CREG",40);  //enter the pin
    wait_gprs_response("bla",10);                                             //only for whaite 1 seconds
    write_gprs_command("AT+CMGF=1\r\n","OK\r\n",40);         //text mode
    wait_gprs_response("bla",10);                                             //only for whaite 1 seconds
    write_gprs_command("ATE0\r\n","OK\r\n",40);              //echo off
    gprs_state_machine = GPRS_INIT;
}
bool wait_gprs_response(uint8 *command,uint8 whaiteTime)
{
    uint8 i = 0;
    whaitTimer = 0;
    while (whaitTimer <= whaiteTime)
    {
        if (command[i] == '\0')
        {
            //putInUartBuffer("return1: %d, %d \r\n",seconds,gprs_second);
            //__delay_cycles(4000000);
            return 1;
        }
        else
        {
            if ( RxBuffer_Uart_Head != RxBuffer_Uart_Tail )
            {
                //putInUartBuffer("i=%d,%c-%c",i,command[i],RxBuffer_Uart[countRX-1]);
                if ( command[i] == RxBuffer_Uart[RxBuffer_Uart_Head] ) i++;
                else i=0;
                RxBuffer_Uart_Head++;
                if ( RxBuffer_Uart_Head == SizeOfBuffer ) RxBuffer_Uart_Head=0;
            }
        }
    }
    //putInUartBuffer("return0\r\n");
    return 0;
}
void wait_gprs_loop()
{
    uint8 i,j,k;
    static uint8 topologyCheck = 0;
    if ( RxBuffer_Uart_Head != RxBuffer_Uart_Tail )
    {
       if (topologyCheck == readGPRSCommand)
       {
           for(i=0; i < totalGPRScomands; i++)
           {
                if ( RxBuffer_Uart[RxBuffer_Uart_Head] == GPRSCommands[i][GPRSCommands_Counter[i]] )
                {
                    GPRSCommands_Counter[i]++;
                    if ( GPRSCommands[i][GPRSCommands_Counter[i]] == '\0' )
                    {
                      if ( i == SMS ) {whaitTimer = 0;PresentAnyCommand = true; topologyCheck = readTelephonNumber;}
                      else if ( i == CALL ) {write_gprs_command("ATH\r\n","OK\r\n",1);}              //echo off//to do something}
                    }
                }
                else GPRSCommands_Counter[i] = 0;
           }
       }
       else if (topologyCheck == readTelephonNumber)
       {
           if ( whaitTimer >= timeToInvalidate) {PresentAnyCommand = false; topologyCheck = readGPRSCommand;}//wrong number or number not came
           else
           {
               for (j=0; j < totalTelephoneNumbers; j++)
               {
                   if ( RxBuffer_Uart[RxBuffer_Uart_Head] == telephonNumbers[j][telephonNumbers_Counter[j]] )
                   {
                       telephonNumbers_Counter[j]++;
                       if ( telephonNumbers[j][telephonNumbers_Counter[j]] == '\0' ) { whaitTimer = 0; topologyCheck = readSMScontent; }
                   }
                   else telephonNumbers_Counter[j] = 0;
               }
           }
       }
       else if ( topologyCheck == readSMScontent )
       {
           if ( whaitTimer >= timeToInvalidate) {PresentAnyCommand = false; topologyCheck = readGPRSCommand;}//wrong number or number not came
           else
           {
               for ( k=0;k < NrOfSMSComands; k++ )
               {
                  if ( RxBuffer_Uart[RxBuffer_Uart_Head] == SMSCommands[k][countForEachCommand[k]] )
                  {
                      countForEachCommand[k]++;
                      if (SMSCommands[k][countForEachCommand[k]] == '\0') { listOfCommandsToExecuting[counterHowManyCommands] = k;counterHowManyCommands++;}//newStateOfCommands=k;
                  }
                  else countForEachCommand[k] = 0;
               }
           }
       }
    RxBuffer_Uart_Head++;
    if ( RxBuffer_Uart_Head == SizeOfBuffer ) RxBuffer_Uart_Head=0;
    }
}
bool write_gprs_command(uint8 *command,uint8 *response,uint8 whaiteTime)
{
    uint8 i;
    for(i = 0; command[i] != '\0'; ++i )
    {
        putInUartBuffer(command[i]);
    }
    if ( wait_gprs_response(response,whaiteTime) )return 1;
    else return 0;
}
