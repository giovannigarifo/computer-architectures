#include "dac.h"
#include "LPC17xx.H" /* LPC17xx definitions*/


/*----------------------------------------------------------------------------*/
/* DISCRETE SINUSOID VALUES																									*/
//
// This sinusoid has an aplitude of 1, and a frequency of 440Hz, so a period
// of T = 1/f = 2,27ms. 
//
// This values will be scaled between 3.3V and 0V by the ADC interrupt handler.
//
// Values obtained by the formula: (1+SEN(880*PI*time_quantum))/2
/*----------------------------------------------------------------------------*/

//initially the table used is equal to the reference one
uint16_t SinLaTable[23] = 
{
	500,636,763,869,947,991,998,
	967,901,804,684,550,412,281,
	167,78,21,0,17,71,158,270,400
}; // 23 values


// quantizated voltage values of a sinusoid of 440Hz with amplitude 1 over a period
uint16_t SinLaTableReference[23] = 
{
	500,636,763,869,947,991,998,
	967,901,804,684,550,412,281,
	167,78,21,0,17,71,158,270,400
}; // 23 values


// DAC init
void DAC_init(void){
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
}


//DAC reset: feed 0 to loudspeaker
void DAC_reset(void){
	LPC_DAC->DACR = 0;
}
