/*
*	Pushbuttons -> GPIO Pin
*
*	KEY1 -> P2.12 (EINT1)
*	KEY2 -> P2.11 (EINT2)
*/

// button history information
#define ALREADY_REQUESTED 0
#define NOT_REQUESTED 1

// buttons actions
void BUTTON_init(void);
void BUTTON_pressed(void);

// buttons handlers
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

