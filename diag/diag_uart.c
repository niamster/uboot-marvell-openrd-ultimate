#include <common.h>

#include "diag.h"

#define FIRST_UART_UNDER_TEST  1
#define NO_OF_UARTS_UNDER_TEST 1
#define UART_BUFFER_SIZE       16

int baudrate_test[] = {9600, 19200, 115200};
unsigned char pattern[UART_BUFFER_SIZE] = {0x00, 0xff, 0xaa, 0x55,
                                 0x0f, 0xa5, 0x5a, 0xf0,
				 0x00, 0xff, 0xaa, 0x55,
				 0x0f, 0xa5, 0x5a, 0xf0};
int max_baudrates = sizeof(baudrate_test)/sizeof(int);

int uart_lpbk_test(int internal);

int uart_int_lpbk_test(void)
{
	return uart_lpbk_test(1);
}

int uart_ext_lpbk_test(void)
{
	return uart_lpbk_test(0);
}

/* The test assumes that UART interrupts are disabled */
int uart_lpbk_test(int internal)
{
	char test[2][9] = {"external", "internal"};
	unsigned char c;
	int  i = FIRST_UART_UNDER_TEST;
	int  j, k;

	for(; i <= NO_OF_UARTS_UNDER_TEST; i++)
	{
		for(j = 0; j < max_baudrates; j++)
		{
			printf("\tUART %d %s loopback test on baudrate %6d ", i, test[internal], baudrate_test[j]);

			/* Initialize serial port under test with X baudrate */
			diag_serial_init(i, baudrate_test[j]);

			/* Set internal loopback bit */
			if(internal)
				diag_int_lpbk(i, 1);

			/* Write to UART buffer */
			for(k = 0; k < UART_BUFFER_SIZE; k++)
				diag_uart_putc(i, pattern[k]);

			/* Read from UART buffer */
			for(k = 0; k < UART_BUFFER_SIZE; k++)
			{
				if(diag_uart_getc(i, &c))
				{
					printf("TIMEOUT\n");
					return 1;
				}

				if(pattern[k] != c)
					break;
			}

			if(k != UART_BUFFER_SIZE)
			{
				printf("FAILED\n\t\tExpected data 0x%02X - %d\n", pattern[k], k);
				printf("\t\tActual   data 0x%02X\n\n", c);
				return 1;
			}
			 printf("PASSED\n");
		}
		/* Reset internal loopback bit */
		if(internal)
			diag_int_lpbk(i, 0);
	}
	return 0;
}
