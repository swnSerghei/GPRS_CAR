/*
 * interrupts.c
 *
 *  Created on: 05 џэт. 2016 у.
 *      Author: Serghei
 */
#include "interrupts.h"
// TA0_A1 Interrupt vector
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
{
      if (TA0IV & TA0IV_TAIFG)  // Vector 10:  TAIFG
      {
          OverFlowOcured = true;
          if ( KarStastes == motorStart )
          {
              OverflowOcuredInEngineStarted++;
              if ( OverflowOcuredInEngineStarted == STOPENGINEAFTER_X_TIMES_OVERFLOWS )
              {
                  OverflowOcuredInEngineStarted = 0;
                  if ( counterExecutedCommands )
                  {
                      KarStastes = Key3fail1;
                      StateOfCommands &= ~(uint32)1<<engine_start;
                      waitKeyTimer = 0;
                      retryersToStartEngine++;
                      counterExecutedCommands--;
                      listOfCommandsToExecuting[counterExecutedCommands] = engine_start;
                  }
                  else
                  {
                      counterHowManyCommands++;
                      waitKeyTimer = 0;
                      KarStastes = Key3fail1;
                      StateOfCommands &= ~(uint32)1<<engine_start;
                      retryersToStartEngine++;
                      listOfCommandsToExecuting[counterExecutedCommands] = engine_start;
                      allCommandsExecuted = false;
                  }
              }
          }
      }
      TA0IV &= ~TA0IV_TAIFG;
      __enable_interrupt();
}


#pragma vector=TIMER1_A0_VECTOR	//timer
__interrupt void Timer1_A1 (void)
{
    static uint8 tmpCounter=0;
    tmpCounter++;
    whaitTimer++;
    waitKeyTimer++;
    waitPanelTimer++;
    tmpCounterForCalculateKeyTime++;
    tmpKey3Time++;
    if ( tmpCounter == 10 )
    {
        tmpCounter = 0;
        seconds++;
        whaitTimer++;
        if (seconds == 60)
        {
            seconds=0;minuts++;
            if (StateOfCommands & ((uint32)1<<parkigLight_start))
            {
                counter_StayActive_ParckingLight++;
                if (counter_StayActive_ParckingLight == StayActive_ParckingLight)
                {
                    StateOfCommands &= ~((uint32)1<<parkigLight_start);
                    P4OUT &= ~parkingLightPin;
                    //print("parkigLight_stop\r\n");
                }
            }
            if (StateOfCommands & ((uint32)1<<engine_start))
            {
                counter_StayActive_EngineStart++;
                if (counter_StayActive_EngineStart == StayActive_EngineStart)
                {
                    StateOfCommands &= ~((uint32)1<<engine_start);
                    P1OUT &= ~( KEY3_ENABLE + KEY2_ENABLE + KEY1_ENABLE );
                    KarStastes = KeysOff;
                    //print("engine_stoped\r\n");
                }
            }
        }
        if (minuts == 60)
        {
            minuts=0;hours++;
        }
        wakeupTimer++;
        if ( wakeupTimer == WAKEUP_AFTER_X_SECONDS && systemState == sleepMode )
        {
            systemState = wakeUpByTimerState;
            wakeUp();
            LPM1_EXIT;
        }
        else if ( wakeupTimer == (WAKEUP_AFTER_X_SECONDS + STAY_AWAKE) && systemState == wakeUpByTimerState )
        {
            if ( (!PresentAnyCommand) && allCommandsExecuted && KarStastes != motorStart)
            {
                systemState = sleepMode;
            }
            else wakeupTimer--;
            //go_to_sleep();
        }
        if ( (KarStastes != KeysOff && systemState != wakeUpByKey2State) && (!shifterState() || !parkingState()) )
        {
            if ( listOfCommandsToExecuting[counterExecutedCommands] == engine_start ) counterExecutedCommands++;
            P1OUT &= ~( KEY3_ENABLE + KEY2_ENABLE + KEY1_ENABLE );
            StateOfCommands &= ~((uint32)1<<engine_start);
            KarStastes = KeysOff;
            //print("cancel to start engine: s=%d,p=%d\r\n",shifterState(),parkingState());
        }
    }
    //if ( (P2IN & KEY2_DIAG) && interruptByKey1_treated )  {systemState = wakeUpByKey2State; interruptByKey1_treated = false;}
 	__enable_interrupt();
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void)
{
	if ( P1IFG & KEY2_DIAG ) //on rising
	{
	    P1IFG &= ~KEY2_DIAG;
	    if (P1IN & KEY2_DIAG)
	    {
            P1IES |= KEY2_DIAG;
            interruptByKey2_treated = false;
            if ( gprs_state_machine != GPRS_INIT )
            {
                wakeUp();LPM1_EXIT;
                //print("Key2 ON\r\n");
            }
            tmpCounterForCalculateKeyTime = 0;
            systemState = wakeUpByKey2State;
	    }
        else//on faling
        {
            P1IES &= ~KEY2_DIAG;

            //if ( gprs_state_machine == GPRS_INIT ) wakeupTimer = WAKEUP_AFTER_X_SECONDS + 1;
            //print("Key2 OFF\r\n");
            systemState = wakeUpByTimerState;
        }
	}

	if (P1IFG & KEY3_DIAG)
    {
        P1IFG &= ~KEY3_DIAG;
        if ( P1IN & KEY3_DIAG )//on rising
        {
            P1IES |= KEY3_DIAG;
            //print("Key3 ON\r\n");
            tmpKey3Time=0;
            P2IE |= ROTATION_PIN;

            if (carLearn == NotLearn && (tmpCounterForCalculateKeyTime < OVERFLOW_TIME_BETWEEN_KEYS) )
            {
                TA0CTL |= MC_2;
                TA0IV &= ~TA0IV_TAIFG;
                TA0CTL |= TAIE ;
                if (carLearnTimes < LEARNING_TIMES)
                {

                    if (!timeBetween2Key_3Key) {timeBetween2Key_3Key+=tmpCounterForCalculateKeyTime;}
                    else {timeBetween2Key_3Key+=tmpCounterForCalculateKeyTime;timeBetween2Key_3Key/=2;}
                    //print("carLearnTimes: %d, %d\r\n",carLearnTimes,timeBetween2Key_3Key);
                    carLearnTimes++;
                }
            }
        }
        else //on faling
        {
            P1IES &= ~KEY3_DIAG;

            //print("Key3 OFF\r\n");
            if ( carLearn != Learn )
            {
                if (carLearnTimes == LEARNING_TIMES)
                {
                    carLearn = Learn;
                    avgRotationSpeed += avgRotationSpeed/50;// add error 2%
                    needToSendSMS = true;
                    //print("car learned with:%d rotation\r\n", avgRotationSpeed);
                }
                if ( holdKey3Time ) {holdKey3Time += tmpKey3Time; holdKey3Time >>= 1;}
                else holdKey3Time += tmpKey3Time;
            }
        }
    }

    __enable_interrupt();
}

#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void)
{
    if ( P2IFG & ROTATION_PIN )
        {
            P2IFG &= ~ROTATION_PIN;
            if ( !OverFlowOcured )
            {
                if ( carLearn != Learn )
                {
                    if ( avgRotationSpeed ) {avgRotationSpeed+= TA0R;avgRotationSpeed>>=1;}
                    else
                    {
                        avgRotationSpeed = TA0R;
                    }
                }
                else
                {
                    if ( avgRotationSpeedValidate ) { avgRotationSpeedValidate += TA0R; avgRotationSpeedValidate>>=1; }
                    else avgRotationSpeedValidate = TA0R;
                    if ( avgRotationSpeedValidate < ( avgRotationSpeed ) )
                    {
                        OverRotationTimeValidation--;
                        if ( KarStastes == Key3on && !OverRotationTimeValidation ) KarStastes = motorStart;
                    }
                }
            }
            else { OverFlowOcured = false;}
            TA0R = 1;
        }
    __enable_interrupt();
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
	if (IFG2 & UCA0TXIFG) {sendNextUartBufferElement_fromInterrupt();IFG2 &= ~UCA0TXIFG;}
	//if (IFG2 & UCA0RXIFG) {Uart_RX_Interrupt();IFG2 &= ~UCA0RXIFG;}
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
