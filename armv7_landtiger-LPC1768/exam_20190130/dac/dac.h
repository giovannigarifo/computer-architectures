#include <string.h>
#include "lpc17xx.h"

/*----------------------------------------------------------------------------
  ADC control functions

This library controls the ADC of the landtiger board:

* 10 bit DAC (2^10 quantization values)
* Precision: 1024 levels between 0 and 3.3V
* maximum update rate of 1MHz (we can provide a new value every MHz)

*----------------------------------------------------------------------------*/



//number of ticks of each sinusoid
#define SIN_LA_TICKS 23

// lib_dac
extern void DAC_init(void);
extern void DAC_reset(void);

// funct_dac
extern void DAC_feed_loudspeaker(void);
