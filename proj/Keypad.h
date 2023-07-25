#ifndef KEYPAD
#define KEYPAD

#include "delay.h"
#include "tm4c123gh6pm.h"

void KEYPAD_init(void);		/*initialize the Keypad pins*/
unsigned char KEYPAD_getKey(void);	
/* this function does:
1. check if key is pressed or NOT
2. set the keypad key-map
3. know the pressed key
*/


void PORTF_init(void);		/*initialize PORTF pins*/
//using PORTF RGB LEDs:
/*flashing with user input...
RED: if ERROR
GREEN: if operation succeeded */
int RG_LED(char C);

#endif
