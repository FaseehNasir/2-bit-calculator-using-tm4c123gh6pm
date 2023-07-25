#include "Keypad.h"
#include "LCD.h"

void LCD_command(unsigned char command);
void LCD_data(unsigned char data);

int main(void) 
{

	  /* used variables */
    unsigned char input = '0';
    int validInput = 0;
    int num = 0;
    int total = 0;
    int result=0;
    char input_array[6];
    int num_array[2];
    static int i = 0;
    char x;
	  static int j = 0;
	  static int k = 0;
    /* initialize used utilities and ports */
    KEYPAD_init();
    PORTF_init();

    /* main loop */
    while (1) 
			{
        input = KEYPAD_getKey();
        validInput = RG_LED(input);

        if (validInput == 1)
					{
            if (input == 'C') 
							{
                LCD_command(0x01);
                LCD_command(0x0F);
							}
            else
							{
                if (i < 6)
									{
                    input_array[i] = input;
                    LCD_data(input);
                    i++;
									}
                for(j = 0; j < 6;j++)
									{
                    if((input_array[j] >= '0') & (input_array[j] <= '9'))
											{
                        num = input_array[j] - '0';
                        total = total*10 + num;
											}
                    else
											{
                        if(k<=1)
													{
                            num_array[k] = total;
                            k++;
													}
											}
									}

                /* apply operation */
                i = 2;
                x = input_array[i];
                switch (x) 
									{
                    case '+':
                        result = num_array[0] + num_array[1];
                        break;
                    case '-':
                        result = num_array[0] - num_array[1];
                        break;
                    case '*':
                        result = num_array[0] * num_array[1];
                        break;
                    case '/':
                        result = num_array[0] / num_array[1];
                        break;
									}

                /* display data */
                i = 0;
                while(i <= 1) 
									{
                    input = result / 10;
                    LCD_data(input);
                    input = result % 10;
                    LCD_data(input);
                    i++;
									}
							}
					}
        delay_Us(500000);
			}
}
