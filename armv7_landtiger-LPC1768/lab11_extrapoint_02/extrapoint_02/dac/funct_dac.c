#include "dac.h"
#include "LPC17xx.H" /* LPC17xx definitions*/


/*----------------------------------------------------------------------------*/
/*  DISCRETE SINUSOID VALUES																									*/
/*----------------------------------------------------------------------------*/
extern uint16_t SinLaTable[];

/*----------------------------------------------------------------------------*/
/*  Feed the loudspeaker with a sinusoid value  															*/
/*----------------------------------------------------------------------------*/
// I want to reproduce a LA=440Hz, given that I have 23 samples for the sinusoid,
// a new value must be feeded to the DAC every .
//
// I've choosed the LA because it's used by car clacson and it's easily distinguishable.	
//
// f=440Hz, 1/f = 2,27ms is the period of the LA sinusoid
//
// timer must interrupt every 2,27ms/23 = 0,098ms = 98us
//
// count = time * frequency = 98*10^-6 * 25MHz = 98*10^-6*25*10^6 = 2450 => Hexadecimal 0x992
void DAC_feed_loudspeaker(void){
	
	static int ticks = 0;
	
	LPC_DAC->DACR = SinLaTable[ticks]<<6;
	ticks++;
	
	if(ticks==SIN_LA_TICKS) 
		ticks=0;
}
