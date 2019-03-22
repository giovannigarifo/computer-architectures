/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {

  LPC_PINCON->PINSEL3 &= ~(3<<18);	//PIN mode GPIO (00b value per P1.25)
	
	//LPC_GPIO1->FIODIR   &= ~(1<<25);	//P1.25 Input (joysticks on PORT1 defined as Input) -> SELECT JOYSTICK
  //LPC_GPIO1->FIODIR   &= ~(1<<26);	//P1.26 Input (joysticks on PORT1 defined as Input) -> DOWN JOYSTICK
	LPC_GPIO1->FIODIR   &= ~(1<<27);	//P1.27 Input (joysticks on PORT1 defined as Input) -> LEFT JOYSTICK
	LPC_GPIO1->FIODIR   &= ~(1<<28);	//P1.28 Input (joysticks on PORT1 defined as Input) -> RIGHT JOYSTICK
  //LPC_GPIO1->FIODIR   &= ~(1<<29);	//P1.29 Input (joysticks on PORT1 defined as Input) -> UP JOYSTICK

	
}
