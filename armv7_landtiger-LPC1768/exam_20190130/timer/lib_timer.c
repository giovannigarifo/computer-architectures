/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        atomic functions to be used by higher sw levels
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"

/*----------------------------------------------------------------------------
  STATUS OF THE LEDS
 *----------------------------------------------------------------------------*/
extern unsigned char LED_STATUS;


/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer, start counting
**
** parameters:			timer number: 0 or 1  or 2 or 3
** Returned value:		None
**
******************************************************************************/
void enable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
		LPC_TIM0->TCR = 1; // enable timer 0
  }
  else if ( timer_num == 1)
  {
		LPC_TIM1->TCR = 1; //enable timer 1
  }
	else if (timer_num == 2)
  {
		LPC_TIM2->TCR = 1; //enable timer 2
  }
	else if ( timer_num == 3)
	{
		LPC_TIM3->TCR = 1; //enable timer 2
	}
}


/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer, stop counting
**
** parameters:			timer number: 0 or 1 or 2 or 3
** Returned value:		None
**
******************************************************************************/
void disable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
		LPC_TIM0->TCR = 0;
  }
  else if ( timer_num == 1 )
  {
		LPC_TIM1->TCR = 0;
  }
	else if (timer_num == 2)
	{
		LPC_TIM2->TCR = 0;
	}
	else if (timer_num == 3)
	{
		LPC_TIM3->TCR = 0;
	}
	
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer, reset the TCR, doesn't stop counting!
**
** parameters:			timer number: 0 or 1 or 2
** Returned value:		None
**
******************************************************************************/
void reset_timer( uint8_t timer_num )
{
  uint32_t regVal;

  if ( timer_num == 0 )
  {
		regVal = LPC_TIM0->TCR;
		regVal |= 0x02;
		LPC_TIM0->TCR = regVal;
  }
  else if (timer_num == 1)
  {
		regVal = LPC_TIM1->TCR;
		regVal |= 0x02;
		LPC_TIM1->TCR = regVal;
  }
	else if (timer_num == 2)
  {
		regVal = LPC_TIM2->TCR;
		regVal |= 0x02;
		LPC_TIM2->TCR = regVal;
  }
	else if (timer_num == 3)
  {
		regVal = LPC_TIM3->TCR;
		regVal |= 0x02;
		LPC_TIM3->TCR = regVal;
  }

  return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		initialize the timer MCR (mode of operation) and MR0 (value to be matched) registers
**
**
** parameters:			timer number: 0 or 1 or 2
** Returned value:		None
**
******************************************************************************/

uint32_t init_timer ( uint8_t timer_num, uint32_t TimerInterval )
{
  if ( timer_num == 0 ) //initialize timer zero
  {
	LPC_TIM0->MR0 = TimerInterval; //timer zero should match value of TimerInterval

//*** <<< Use Configuration Wizard in Context Menu >>> ***
// <h> timer0 MCR
//   <e.0> MR0I
//	 <i> 1 Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC. 0
//	 <i> 0 This interrupt is disabled
//   </e>
//   <e.1> MR0R
//	 <i> 1 Reset on MR0: the TC will be reset if MR0 matches it.
//	 <i> 0 Feature disabled.
//   </e>
//   <e.2> MR0S
//	 <i> 1 Stop on MR0: the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC
//	 <i> 0 Feature disabled.
//   </e>
//   <e.3> MR1I
//	 <i> 1 Interrupt on MR1: an interrupt is generated when MR0 matches the value in the TC. 0
//	 <i> 0 This interrupt is disabled
//   </e>
//   <e.4> MR1R
//	 <i> 1 Reset on MR1: the TC will be reset if MR0 matches it.
//	 <i> 0 Feature disabled.
//   </e>
//   <e.5> MR1S
//	 <i> 1 Stop on MR1: the TC and PC will be stopped and TCR[1] will be set to 0 if MR1 matches the TC
//	 <i> 0 Feature disabled.
//   </e>
//   <e.6> MR2I
//	 <i> 1 Interrupt on MR2: an interrupt is generated when MR2 matches the value in the TC.
//	 <i> 0 This interrupt is disabled
//   </e>
//   <e.7> MR2R
//	 <i> 1 Reset on MR2: the TC will be reset if MR2 matches it.
//	 <i> 0 Feature disabled.
//   </e>
//   <e.8> MR2S
//	 <i> 1 Stop on MR2: the TC and PC will be stopped and TCR[2] will be set to 0 if MR2 matches the TC
//	 <i> 0 Feature disabled.
//   </e>
//   <e.9> MR3I
//	 <i> 1 Interrupt on MR3: an interrupt is generated when MR3 matches the value in the TC.
//	 <i> 0 This interrupt is disabled
//   </e>
//   <e.10> MR3R
//	 <i> 1 Reset on MR3: the TC will be reset if MR3 matches it.
//	 <i> 0 Feature disabled.
//   </e>
//   <e.11> MR3S
//	 <i> 1 Stop on MR3: the TC and PC will be stopped and TCR[3] will be set to 0 if MR3 matches the TC
//	 <i> 0 Feature disabled.
//   </e>
	LPC_TIM0->MCR = 7;
// </h>
//*** <<< end of configuration section >>>    ***

	NVIC_EnableIRQ(TIMER0_IRQn);
	return (0);
  }
  else if ( timer_num == 1 ) // initialize timer 1
  {
		LPC_TIM1->MR0 = TimerInterval;
		LPC_TIM1->MCR = 3;				/* Interrupt and Reset on MR1 */

		NVIC_EnableIRQ(TIMER1_IRQn);
		return (1);
  } 
	else if ( timer_num == 2 ) // initialize timer 2
  {
		LPC_SC->PCONP |=  (1<<22); /* Enable power to TIMER2, at reset it's powered down*/
		
		LPC_TIM2->MR0 = TimerInterval;
		LPC_TIM2->MCR = 3;				/* Interrupt, reset on MR2 */

		NVIC_EnableIRQ(TIMER2_IRQn);
		return (2);
  }
	else if ( timer_num == 3 ) // initialize timer 3
  {
		LPC_SC->PCONP |=  (1<<23); /* Enable power to TIMER3, at reset it's powered down*/
		
		LPC_TIM3->MR0 = TimerInterval;
		LPC_TIM3->MCR = 3;				/* Interrupt, reset on MR3 */

		NVIC_EnableIRQ(TIMER3_IRQn);
		return (3);
  }

	return 4; //wrong timer number received, return 4 to signal wrong parameter
}


/******************************************************************************
** Function name:		powerdown_timer
**
** Descriptions:		Powerdown the timer
**
** parameters:			timer number: 0 or 1  or 2
** Returned value:		None
**
******************************************************************************/
void powerdown_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
		LPC_SC->PCONP |=  (1<<1); //Disable power to TIMER0
  }
  else if ( timer_num == 1)
  {
		LPC_SC->PCONP |=  (1<<2); //Disable power to TIMER1
  }
	else if (timer_num == 2)
  {
		LPC_SC->PCONP |=  (1<<22); //Disable power to TIMER2
  }
	else if (timer_num == 3)
  {
		LPC_SC->PCONP |=  (1<<23); //Disable power to TIMER3
  }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
