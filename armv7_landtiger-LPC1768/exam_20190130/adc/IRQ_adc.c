/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../dac/dac.h"

/*----------------------------------------------------------------------------*/
/*  DISCRETE SINUSOID VALUES																									*/
/*----------------------------------------------------------------------------*/
extern uint16_t SinLaTable[];
extern uint16_t SinLaTableReference[];

unsigned short AD_current;   			 /* current converted value */
unsigned short AD_last = 0xFF;     /* Last converted value */

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler(void) {
  	
	/* Read Conversion Result from "AD global data register" */
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);
  
	if(AD_current != AD_last){
		
		// scale the SinLaTable reproduced by the loudspeaker proportionally to the AD_current
		// the array values are between 0V and 1V, so we scale them in a range 0V-3.3V, the scaling
		// factor is the AD_current value: the voltage read from the potentiometer.
		int i = 0;
		float scaleFactor = (float)((float)AD_current/(float)1240); //1240= (quantizationLevels)/(maxVoltage) = 4096/3,3
		
		// overvoltage 
		if( scaleFactor > 3.3f) scaleFactor = 3.3f;
		
		// scale the sinusoid
		for(i=0; i < SIN_LA_TICKS; i++){
			SinLaTable[i] = SinLaTableReference[i]*scaleFactor; 
		}
		
		AD_last = AD_current;
  }
	
}
