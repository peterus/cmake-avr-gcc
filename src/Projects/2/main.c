/*
 * UART_Test.c
 *
 * Created: 26.10.2014 19:01:09
 *  Author: Peter Buchegger
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "System.h"
#include "com/UART.h"

int main(void)
{
	UART_Init(UART_BAUD_SELECT(9600, F_CPU));
	sei();

	while(1)
	{
		printf("Hello world!\n");
		sleep(1000);
	}
}

