																			 /*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_led, funct_led .c files
** Correlated files:    lib_led.c, funct_led.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/*
*	Leds connected to GPIO Port 2.x Pin
*
*	LD11 (rightmost on board) -> P2.0
*	LD10 -> P2.1
*	LD9 -> P2.2
*	LD8 -> P2.3
*	LD7 -> P2.4
*	LD6 -> P2.5
*	LD5 -> P2.6
*	LD4 (leftmost on board) -> P2.7
*/

/* Possible states of the leds */
#define	LED_ALL_OFF 0

/* lib_led */
void LED_init(void);
void LED_deinit(void);

/* funct_led */
void LED_On (unsigned int num);
void LED_Off (unsigned int num);
void LED_Out(unsigned int value); //output the binary representation of value on the leds




