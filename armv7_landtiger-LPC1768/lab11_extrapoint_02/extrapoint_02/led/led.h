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
*	Led light -> GPIO Pin
*
*	LD11 -> P2.0
*	LD10 -> P2.1
*	LD9 -> P2.2
*	LD8 -> P2.3
*	LD7 -> P2.4
*	LD6 -> P2.5
*	LD5 -> P2.6
*	LD4 -> P2.7
*/

/* LED STATUS */
#define	CARRED_PEDGREEN 0
#define	CARRED_PEDFLASHGREEN 1
#define	CARGREEN_PEDRED	2
#define CARYELLOW_PEDRED	3
#define	MAINTENANCE 4


/* lib_led */
void LED_init(void);
void LED_deinit(void);

/* funct_led */
void LED_On (unsigned int num);
void LED_Off (unsigned int num);
void LED_Out(unsigned int value);

/* Semaphore on-off functions for car and pedestrians semaphores */
void LED_CarRed_PedGreen(void);
void LED_CarRed_PedOff(void);
void LED_CarGreen_PedRed(void);
void LED_CarYellow_PedRed(void);
void LED_CarOff_PedOff(void);




