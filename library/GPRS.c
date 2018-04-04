/*
 * GPRS.c
 *
 *  Created on: Mar 21, 2017
 *      Author: uidg6243
 */
#include "GPRS.h"
const uint8 *SMSCommands[]=
{
    {"parkingLight_start"},
    {"parkingLight_stop"},
    {"key1_start"},
    {"key1_stop"},
    {"engine_start"},
    {"engine_stop"},
    {"recirculare_start"},
    {"recirculare_stop"},
    {"ac_start"},
    {"ac_stop"},
    {"luneta_start"},
    {"luneta_stop"},
    {"parbriz_start"},
    {"parbriz_stop"},
    {"ventilator_3pozitie"},
    {"ventilator_4pozitie"},
    {"ventilator_off"},
    {"unlock_doors"},
    {"delaySeconds"},
    //without verify who sender, directly verify if present any sms command =====> defined by howManyCommandsWithoutVerifySender
    {"report"},
    {"gps_off"},
    {"gps_on"},
    {"gps_get"},
};

const uint8 *GPRSCommands[]=
{
    {"+CMT: \""},
    {"RING"},
    {"GPRMC"}
};

uint8 *telephonNumbers[]=
{
 {"+40757294327"},
 {"+37379548801"}
};
void init_GPRS_only_on_PowerOn()
{
    uint8 i;
    P4DIR |= PWR_GPRS;
    PresentAnyCommand = false;
    counterHowManyCommands=0;
    counterExecutedCommands=0;
    gps_get_From_Counter=0;
    for (i=0;i<NrOfSMSComands;i++)
    {
        listOfCommandsToExecuting[i] = 0;
    }
}
void powerON_GPRS()
{
    //print("Activate pin: %d, %d, %d \r\n",seconds,minuts,hours);
    P4OUT |= PWR_GPRS;
    whaitTimer=0;while (whaitTimer < POWER_ON_BUTTON){}
    P4OUT &= ~PWR_GPRS;
    wait_gprs_response("CINIT:",60);                     //whaite CREG
    wait_gprs_response("CINIT:",60);                     //whaite CREG
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
                      if ( i == SMS )
                      {
                          GPRSCommands_Counter[i]=0;whaitTimer = 0; PresentAnyCommand = true; topologyCheck = readTelephonNumber;
                          gps_get_From_Counter = 0;telephone_complete=false;
#if debugMode == 1
                          print("Present SMS command\r\n");
#endif
                      }
                      else if ( i == CALL )
                      {
                          GPRSCommands_Counter[i]=0;write_gprs_command("ATH\r\n","OK\r\n",1);
                      }
                      else if ( i == GPS_STRING )
                      {
                          whaitTimer = 0; SMScontent_Counter=0;
                          __delay_cycles(1000000);
                          while (whaitTimer <= 10 && RxBuffer_Uart[RxBuffer_Uart_Head] != '\n' && SMScontent_Counter < SizeOfMSG )//1 second wait string
                          {
                              if ( RxBuffer_Uart_Tail !=  RxBuffer_Uart_Head )
                              {
                                  SMScontent[SMScontent_Counter] = RxBuffer_Uart[RxBuffer_Uart_Head];
                                  SMScontent_Counter++;
                                  RxBuffer_Uart_Head++;if ( RxBuffer_Uart_Head == SizeOfBuffer ) RxBuffer_Uart_Head=0;
                                  if (SMScontent_Counter >= (SizeOfMSG) ) SMScontent_Counter=0;
                              }
                          }
                          SMScontent[SMScontent_Counter] = '\0';
                          GPRSCommands_Counter[i]=0;
                      }
                    }
                }
                else GPRSCommands_Counter[i] = 0;
                }
       }
       else if (topologyCheck == readTelephonNumber)
       {
               for (j=0; j < totalTelephoneNumbers; j++)
               {
                   if ( RxBuffer_Uart[RxBuffer_Uart_Head] == telephonNumbers[j][telephonNumbers_Counter[j]] )
                   {
                       telephonNumbers_Counter[j]++;
                       if ( telephonNumbers[j][telephonNumbers_Counter[j]] == '\0' )
                       {
                           telephonNumbers_Counter[j] = 0;
                           whaitTimer = 0;
                           PresentAnyCommand = true;
                           topologyCheck = readSMScontent;
                           gps_get_From[gps_get_From_Counter+1] = '\0';
#if debugMode == 1
                           print("Present tel Number command\r\n");
#endif
                       }
                   }
                   else telephonNumbers_Counter[j] = 0;
               }
               if ( RxBuffer_Uart[RxBuffer_Uart_Head] != '\"' &&  telephone_complete == false)
               {
                   gps_get_From[gps_get_From_Counter] = RxBuffer_Uart[RxBuffer_Uart_Head];
                   gps_get_From_Counter++;
                   if ( gps_get_From_Counter == (gps_get_From_Length) ) gps_get_From_Counter=0;
               }
               else
               {
                   gps_get_From[gps_get_From_Counter] = '\0';telephone_complete = true;
                   for ( k=(NrOfSMSComands - howManyCommandsWithoutVerifySender);k < NrOfSMSComands; k++ )
                  {
                     if ( RxBuffer_Uart[RxBuffer_Uart_Head] == SMSCommands[k][countForEachCommand[k]] )
                     {
                         countForEachCommand[k]++;
                         if (SMSCommands[k][countForEachCommand[k]] == '\0')
                         {
                             listOfCommandsToExecuting[counterHowManyCommands] = k; counterHowManyCommands++; allCommandsExecuted = false; countForEachCommand[k]=0;

                #if debugMode == 1
                             print("Present command from\r\n");
                #endif
                         }
                     }
                     else countForEachCommand[k] = 0;
                  }
               }
       }
       else if ( topologyCheck == readSMScontent )
       {
               for ( k=0;k < NrOfSMSComands; k++ )
               {
                  if ( RxBuffer_Uart[RxBuffer_Uart_Head] == SMSCommands[k][countForEachCommand[k]] )
                  {
                      countForEachCommand[k]++;
                      if (SMSCommands[k][countForEachCommand[k]] == '\0')
                      {
                          listOfCommandsToExecuting[counterHowManyCommands] = k; counterHowManyCommands++; allCommandsExecuted = false; countForEachCommand[k]=0;

#if debugMode == 1
                          print("Present command\r\n");
#endif
                      }
                  }
                  else countForEachCommand[k] = 0;
               }
       }
    RxBuffer_Uart_Head++;
    if ( RxBuffer_Uart_Head == SizeOfBuffer ) RxBuffer_Uart_Head=0;
    }
    else
    {
        if ( whaitTimer >= timeToInvalidate && PresentAnyCommand )
        {
            PresentAnyCommand = false; topologyCheck = readGPRSCommand;

#if debugMode == 1
            print("Timeout command\r\n");
#endif
        }
    }
}
bool write_gprs_command(uint8 *command,uint8 *response,uint8 whaiteTime)
{
    while ( *command != '\0' )
    {
        putInUartBuffer(*command);
        command++;
    }
    if ( wait_gprs_response(response,whaiteTime) )return 1;
    else return 0;
}
void gprs(uint8 action)
{
    if ( action == activate )
    {
        write_gprs_command("AT+GPSRD=10\r\n","OK",10);//NEMA information N seconds output ONE time by AT serial port, actual use of n into numbers
        write_gprs_command("AT+GPS=1\r\n","OK",10);//open GPS
        write_gprs_command("AT+AGPS=1\r\n","OK",10);//After you open the GPS/AGPS, default information from NEMA GPS_TXD output pins with a 9600 baud rate, if make NEMA output by AT serial port,can be used AT +GPSRD.
    }
    else if ( action == deactivate )
    {
        if ( write_gprs_command("AT+GPS=0\r\n","OK",10) == 0 || write_gprs_command("AT+AGPS=0\r\n","OK",10) )
        {
            write_gprs_command("AT+GPS=0\r\n","OK",10);
            write_gprs_command("AT+AGPS=0\r\n","OK",10);
        }
    }
    else if ( action == gps_send_via_sms )
    {
        //write_gprs_command("AT+CMGS=\"0040757294327\"\r\n",">\r\n",10);
        print("AT+CMGS=\"%s\"\r\n",gps_get_From);
        wait_gprs_response(">\r\n",10);
        wait_gprs_response("bla",10);                                             //only for whaite 1 seconds
        print(SMScontent);
        putInUartBuffer(26);//ctrl+z
        wait_gprs_response("OK",10);
    }
    counterExecutedCommands++;
}
