#include <string.h>

/*----------------------------------------------------------------------------
  ADC control functions

This library controls the ADC of the landtiger board:

* 12 bit ADC (2^12 quantization values)
* input multiplexing among 8 pins, in our case only pin enabled is the one
	connected to the potentiometer
* convertion rate of 200KHz (a sample is produced every 200KHz)

* Potentiometer VR1 is connected to analog channel P1.31 (AD0.5). Potentiomenter is enabled by jumper JP13
* Potentiometer produce a voltage between 0V and 3.3V, feeed to the ADC


*----------------------------------------------------------------------------*/



/* lib_adc.c */
void ADC_init (void);

void ADC_start_conversion (void);
void ADC_stop_conversion (void);

void ADC_enable (void);
void ADC_disable (void);


/* IRQ_adc.c */
void ADC_IRQHandler(void);
