#include "Keypad.h"


/*initialize the Keypad pins*/
void KEYPAD_init() 
{
	SYSCTL_RCGCGPIO_R |= 0x04;   /* enable clock to GPIOC */ 
	SYSCTL_RCGCGPIO_R |= 0x10;   /* enable clock to GPIOE */
	GPIO_PORTE_DIR_R |= 0x0F;     /* set row pins 3-0 as output */
	GPIO_PORTE_DEN_R |= 0x0F;     /* set row pins 3-0 as digital pins */
	GPIO_PORTE_ODR_R |= 0x0F;     /* set row pins 3-0 as open drain */
	GPIO_PORTC_DIR_R &= ~0xF0;    /* set column pin 7-4 as input */
	GPIO_PORTC_DEN_R|= 0xF0;     /* set column pin 7-4 as digital pins */
	GPIO_PORTC_PUR_R |= 0xF0;     /* enable pull-ups for pin 7-4*/ 
}


/* helpful function used for checking if a key is pressed*/

unsigned char keypad_hit(void) 
{
	int col;
	GPIO_PORTE_DATA_R=0;
	col=GPIO_PORTC_DATA_R & 0xF0;
	if(col==0xF0)
		return 0;
	else
		return 1;
}


/* this function does the following:
    1. check if key is pressed or NOT
    2. set the keypad key-map
    3. know the pressed key
*/
unsigned char KEYPAD_getKey() 
{
	const unsigned char keymap[4][4] = 
	{{ '1', '2', '3', '+'},
	{ '4', '5', '6', '-'},
	{ '7', '8', '9', '*'},
	{ 'C', '0', '=', '/'}};
	int row, col; /* check to see any key pressed first */	
	GPIO_PORTE_DATA_R = 0;               /* enable all rows */ 
	col = GPIO_PORTC_DATA_R & 0xF0;      /* read all */

	while (1)
	{ 
		row = 0;
		GPIO_PORTE_DATA_R = 0x0E;        /* enable row 0 */
		delay_Us(2);                     /* wait for signal to settle */ 
		col = GPIO_PORTC_DATA_R & 0xF0;
		if (col != 0xF0)
		{
			delay_Us(500000);
			break;
		}
		delay_Us(2);
		
		row = 1;
		GPIO_PORTE_DATA_R = 0x0D;        /* enable row 1 */ 
		delay_Us(2);                     /* wait for signal to settle */ 
		col = GPIO_PORTC_DATA_R & 0xF0;
		if (col != 0xF0)
		{
			delay_Us(500000);
			break;
		}
		delay_Us(2);
		
		row = 2; 
		GPIO_PORTE_DATA_R  = 0x0B;        /* enable row 2 */ 
		delay_Us(2);                     /* wait for signal to settle */
		col = GPIO_PORTC_DATA_R & 0xF0; 
		if (col != 0xF0){
			delay_Us(500000);
			break;
		}
		delay_Us(2);
		
		row = 3;
		GPIO_PORTE_DATA_R = 0x07;        /* enable row 3 */
		delay_Us(2);                     /* wait for signal to settle */
		col = GPIO_PORTC_DATA_R & 0xF0;
		if (col != 0xF0)
		{
			delay_Us(500000);
			break;
		}
	}
	switch(col)
	{
		case 0xE0:	//key in column 0
			return keymap[row][0];
		case 0xD0:	//key in column 1
			return keymap[row][1];
		case 0xB0:	//key in column 2
			return keymap[row][2];
		case 0x70:	//key in column 3
			return keymap[row][3];
	}
	return 0;
}


/*initialize PORTF pins*/
void PORTF_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;
    while((SYSCTL_RCGCGPIO_R & 0x20) == 0){/*do nothing*/}
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_DIR_R |= 0x0A;           // PF1, PF3 are OUTPUT
    //GPIO_PORTF_DIR_R &= ~(0x11);  //PF0, PF4 are INPUT
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;
}


//using PORTF RGB LEDs:
/*flashing with user input...
RED: if ERROR
GREEN: if operation succeeded */
int RG_LED(char C) 
{
	//SIGN_array ??
	static char last='0', current='0';
	int VALID = 0;
	current = C;
	if(((last == '+')|(last == '-')|(last == '*')|(last == '/')|(last == '=')) &
		((current == '+')|(current == '-')|(current == '*')|(current == '/')|(current == '='))) 
	GPIO_PORTF_DATA_R = 0x02;	//RED LED
	else {
		GPIO_PORTF_DATA_R = 0x08;	//GREEN LED
		VALID = 1;
	}
	last = C;
	delay_Us(200000);
	GPIO_PORTF_DATA_R = 0x00;
	return VALID;
}
