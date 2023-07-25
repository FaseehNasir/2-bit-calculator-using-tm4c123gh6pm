#include "delay.h"

void delay_Us(unsigned long n) 
{
	unsigned long i , j;
	i=0, j=0;
	while(i < n){
		while(j < 3){ j++;
			//Do Nothing 
		}
		i++;
	}
}
