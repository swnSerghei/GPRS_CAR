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
          if ( KarStastes == motorStarted )
          {
#if debugMode == 1
              print("OVF%d\r\n",OverflowOcuredInEngineStarted);
#endif
              OverflowOcuredInEngineStarted++;
              if ( OverflowOcuredInEngineStarted == STOPENGINEAFTER_X_TIMES_OVERFLOWS )
              {
                  OverflowOcuredInEngineStarted = 0;
                  retryersToStartEngine++;
                  if ( counterExecutedCommands )
                  {
                      KarStastes = Key3fail1; waitKeyTimer = 0;
                      counterExecutedCommands--;listOfCommandsToExecuting[counterExecutedCommands] = engine_start;
                  }
                  else
                  {
                      counterHowManyCommands++;listOfCommandsToExecuting[counterExecutedCommands] = engine_start;
                      KarStastes = Key3fail1; waitKeyTimer = 0;
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
            if ( parkingLightState )
            {
                counter_StayActive_ParckingLight++;
                if (counter_StayActive_ParckingLight == StayActive_ParckingLight)
                {
                    deactivateparkingLight();
#if debugMode == 1
                    print("parkingLight_stop\r\n");
#endif
                }
            }
            if ( KarStastes == motorStarted )
            {
                counter_StayActive_EngineStart++;
                if (counter_StayActive_EngineStart == StayActive_EngineStart)
                {
                    deactivateVentilator(ventilator4 + ventilator3);
                    P1OUT &= ~( KEY1_ENABLE+ KEY2_ENABLE  + KEY3_ENABLE ); KarStastes = KeysOff;
                    deactivateFromPanel(AC);deactivateFromPanel(recirculare);deactivateFromPanel(parbriz);deactivateFromPanel(luneta);
#if debugMode == 1
                    print("engine_stoped\r\n");
#endif
                }
            }
        }
        if (minuts == 60)
        {
            minuts=0;hours++;
            if ( NotActiveTime != SLEEP_PERIODIC_AFTER ) NotActiveTime++;
        }

        wakeupTimer++;
#if debugMode == 1
        if ( systemState == wakeUpByTimerState ) print("wakeupTimer=%d\r\n",wakeupTimer);
#endif
        if ( wakeupTimer == WAKEUP_AFTER_X_SECONDS && systemState == sleepMode )
        {
            systemState = wakeUpByTimerState;
            wakeUp();
            LPM1_EXIT;
        }
        else if ( wakeupTimer == (WAKEUP_AFTER_X_SECONDS + STAY_AWAKE) )
        {
            if ( (!PresentAnyCommand) && allCommandsExecuted && KarStastes != motorStarted && systemState == wakeUpByTimerState )
            {
                if ( NotActiveTime == SLEEP_PERIODIC_AFTER) systemState = sleepMode;
                //systemState = sleepMode;
            }
            wakeupTimer--;
        }
        if ( KarStastes == motorStarted && systemState != wakeUpByKey2State && (!shifterState() || !parkingState()) )
        {
            if ( counterExecutedCommands ) { counterExecutedCommands--; listOfCommandsToExecuting[counterExecutedCommands] = engine_stop; }
            else { counterHowManyCommands++; listOfCommandsToExecuting[counterExecutedCommands] = engine_stop; }
            //P1OUT &= ~( KEY3_ENABLE + KEY2_ENABLE + KEY1_ENABLE );
            //KarStastes = KeysOff;

#if debugMode == 1
            print("cancel to start engine: s=%d,p=%d\r\n",shifterState(),parkingState());
#endif
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
	    if (P1IN & KEY2_DIAG)
	    {
	        NotActiveTime = 0;
	        if (systemState == sleepMode) { wakeUp(); LPM1_EXIT; wakeupTimer = WAKEUP_AFTER_X_SECONDS; }
#if debugMode == 1
	        print("Key2 ON\r\n");
#endif
	        deactivateparkingLight();
            deactivateVentilator(ventilator4 + ventilator3);
            P1OUT &= ~( KEY1_ENABLE+ KEY2_ENABLE  + KEY3_ENABLE ); KarStastes = KeysOff;
            deactivateFromPanel(AC);deactivateFromPanel(recirculare);deactivateFromPanel(parbriz);deactivateFromPanel(luneta);
            retryersToStartEngine = 0;

            if ( carLearn == Learn ) { TA0CTL &= ~(MC_2 + TAIE);P2IE &= ~ROTATION_PIN; }

            tmpCounterForCalculateKeyTime = 0;
            systemState = wakeUpByKey2State;

            P1IES |= KEY2_DIAG;
	    }
        else//on faling
        {
#if debugMode == 1
            print("Key2 OFF\r\n");
#endif
            key2_diag_fallingOcured1 = true;
            key2_diag_fallingOcured2 = true;
            systemState = wakeUpByTimerState;

            P1IES &= ~KEY2_DIAG;
        }
	    P1IFG &= ~KEY2_DIAG;
	}

	if (P1IFG & KEY3_DIAG)
    {

        if ( P1IN & KEY3_DIAG )//on rising
        {
#if debugMode == 1
            print("Key3 ON\r\n");
#endif
            tmpKey3Time=0;
            if (carLearn == NotLearn && key2_diag_fallingOcured1)
            {
                TA0CTL |= MC_2;TA0IV &= ~TA0IV_TAIFG;TA0CTL |= TAIE;P2IE |= ROTATION_PIN;
                key2_diag_fallingOcured1 = false;
                if (carLearnTimes < LEARNING_TIMES)
                {
                    if ( !timeBetween2Key_3Key ) { timeBetween2Key_3Key+=tmpCounterForCalculateKeyTime; }
                    else { timeBetween2Key_3Key+=tmpCounterForCalculateKeyTime;timeBetween2Key_3Key/=2; }
                    carLearnTimes++;
#if debugMode == 1
                    print("carLearnTimes: %d, %d\r\n",carLearnTimes,timeBetween2Key_3Key);
#endif
                }
            }
            P1IES |= KEY3_DIAG;
        }
        else //on faling
        {
#if debugMode == 1
            print("Key3 OFF\r\n");
#endif
            P1IES &= ~KEY3_DIAG;
            if ( carLearn != Learn && key2_diag_fallingOcured2 )
            {
                key2_diag_fallingOcured2 = false;
                //if ( avgRotationSpeed == 0 ) if (carLearnTimes) carLearnTimes--;
                if (carLearnTimes == LEARNING_TIMES)
                {
                    carLearn = Learn;
                    if ( (0xFFFF - (avgRotationSpeed/5)) >  avgRotationSpeed ) avgRotationSpeed = avgRotationSpeed + avgRotationSpeed/5;// add error 20%
#if debugMode == 1
                    print("car learned with:%d rotation\r\n", avgRotationSpeed);
#endif
                }
                if ( holdKey3Time ) {holdKey3Time += tmpKey3Time; holdKey3Time >>= 1;}
                else holdKey3Time += tmpKey3Time;
            }
        }
        P1IFG &= ~KEY3_DIAG;
    }
    __enable_interrupt();
}

#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void)
{
    if ( P2IFG & ROTATION_PIN )
        {
            OverflowOcuredInEngineStarted = 0;
            if ( !OverFlowOcured )
            {
                if ( carLearn != Learn )
                {
                    if ( avgRotationSpeed ) {avgRotationSpeed+= TA0R;avgRotationSpeed>>=1;}
                    else avgRotationSpeed = TA0R;
                }
                else
                {
                    if ( avgRotationSpeedValidate ) { avgRotationSpeedValidate += TA0R; avgRotationSpeedValidate>>=1; }
                    else avgRotationSpeedValidate = TA0R;
                    if ( avgRotationSpeedValidate < avgRotationSpeed )
                    {
                        OverRotationTimeValidation--;
                        if ( KarStastes == Key3on && !OverRotationTimeValidation )
                        {
                            KarStastes = motorStarted;
                        }
                    }
                }
            }
            else { OverFlowOcured = false;}
            TA0R = 1;
            P2IFG &= ~ROTATION_PIN;
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
