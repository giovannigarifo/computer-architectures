/*
*	Pushbuttons connected to GPIO Port 2.x Pin
*
* INT0 -> P2.10 (EINT0)
*	KEY1 -> P2.11 (EINT1)
*	KEY2 -> P2.12 (EINT2)
*/


// buttons initialization
void BUTTON_init(void);

// buttons handlers
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

// buttons actions
void BUTTON_pressed(void);
