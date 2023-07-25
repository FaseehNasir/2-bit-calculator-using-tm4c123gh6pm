#include "LCD.h"

#define RS	0x20	//PORTA bit 5 MASK
#define RW	0x40	//PORTA bit 6 MASK
#define EN	0x80	//PORTA bit 7 MASK

// PORTA >> Control
// PORTB >> Data
/*
void lcd_command(unsigned char command) 
	{
    GPIOA->DATA = 0x00; // Set data pins to 0
    GPIOB->DATA = command; // Send command to data pins
    GPIOA->DATA |= EN; // Set enable pin
    delay_10ms(); // Delay
    GPIOA->DATA &= ~EN; // Clear enable pin
    delay_100ms(); // Delay
}

void lcd_data(unsigned char data) {
    GPIOA->DATA = RS; // Set register select pin
    GPIOB->DATA = data; // Send data to data pins
    GPIOA->DATA |= EN; // Set enable pin
     delay_10ms();// Delay
    GPIOA->DATA &= ~EN; // Clear enable pin
     delay_100ms();// Delay
}
*/
void LCD_command(unsigned char command) 
{
	//control pins::
	GPIO_PORTA_DATA_R = 0x00;		// RS=0 , RW=0
	GPIO_PORTB_DATA_R = command;
	GPIO_PORTA_DATA_R = EN;		// pulse E
	delay_Us(1000);			// enable pulse width
	GPIO_PORTA_DATA_R = 0x00;
	
	if(command < 4)
		delay_Us(2000);
	else
		delay_Us(40);
}

void LCD_data(unsigned char data) 
{
	GPIO_PORTA_DATA_R = RS;	// RS=1 , RW=0
	GPIO_PORTB_DATA_R = data;
	GPIO_PORTA_DATA_R = (EN | RS);	// pulse E
	delay_Us(1000);			// enable pulse width
	GPIO_PORTA_DATA_R = 0x00;
	delay_Us(40);
}
/*
void lcd_init(void) {
    SYSCTL->RCGCGPIO|= 0x01; // Enable Port A clock
	SYSCTL->RCGCGPIO |= 0x02;  //Enable Port B clock
    delay_10ms(); // Delay
    GPIOA->DIR|= 0x06; // Set PA1 and PA2 as output
    GPIOA->DEN |= 0x06; // Set PA1 and PA2 as digital pins
    GPIOB->DIR |= 0xFF; // Set PB0-PB7 as output
    GPIOB->DEN |= 0xFF; // Set PB0-PB7 as digital pins
    lcd_command(0x38); // 8-bit data, 2-line display, 5x7 font
    lcd_command(0x06); // Increment cursor position after each write
    lcd_command(0x0F); // Turn on display, cursor, and blinking
    lcd_command(0x01); // Clear display
    delay_100ms(); // Delay
}
void lcd_display_string(const char *str)
{
    while (*str)
    {
        lcd_data((unsigned char)*str);
        str++;
    }
}
void lcd_display_integer(uint32_t num)
{
    char buffer[12];  // Buffer to hold the integer as a string
    sprintf(buffer, "%lu", num);  // Convert the integer to a string
    lcd_display_string(buffer);
}
*/
void LCD_init(void) 
{
	//prepare ports::
	SYSCTL_RCGCGPIO_R |= 0x03;		// enable clock for PORTA and PORTB
	while((SYSCTL_PRGPIO_R & 0x03) == 0){/*do nothing*/};		// wait for clock
	//PORTA initialization:
	GPIO_PORTA_DIR_R |= 0xE0;			// set PORTA pin 5,6,7 as OUTPUT
	GPIO_PORTA_DEN_R |= 0xE0;			// set PORTA pin 5,6,7 as DIGITAL
	//PORTB initialization:
	GPIO_PORTB_DIR_R |= 0xFF;			// set PORTB pin 0,1,...,6,7 as OUTPUT
	GPIO_PORTB_DEN_R |= 0xFF;			// set PORTB pin 0,1,...,6,7 as DIGITAL
	
	//wake-up the LCD::
	delay_Us(20000);			// wait '>15 ms' after power is applied
	LCD_command(0x30);	// wake-up #1
	delay_Us(5000);				// busy_flag is NOT available
	LCD_command(0x30);	// wake-up #2
	delay_Us(100);			// busy_flag is NOT available
	LCD_command(0x30);	// wake-up #3
	delay_Us(100);			// busy_flag is NOT available
	
	//prepare LCD::
	LCD_command(0x38);			// set 8-bit data
	LCD_command(0x06);			// move cursor right
	LCD_command(0x01);			// clear screen
	LCD_command(0x0F);			// display is ON and cursor blinking
}
