#include <stdint.h>
#include "gpio.h"
#include "uart.h"
//#include "uart.c"

#include <stdio.h>

int main() {

	for (int i = 4; i<= 15;i++) {
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	GPIO->PIN_CNF[17] = 0;
	GPIO->PIN_CNF[26] = 0;


	uart_init();


	/*
	while (1) {
		
		if (!(GPIO->IN & (1 << 17))) {
			//GPIO->OUTCLR = (1 << 13 | 1 << 14 | 1<< 15);
			uart_send('A');	
		}

		else if (!(GPIO->IN & (1 << 26))) {
			//GPIO->OUTSET = (1 << 13 | 1 << 14 | 1<< 15);
			uart_send('B');
		}
	*/

	iprintf("Norway has %d counties.\n\r", 18);

	while (1) {
		if (!(uart_read() == '\0')) {
			int lights_on;

			if (!lights_on) {
				GPIO->OUTSET = (1 << 13 | 1 << 14 | 1<< 15);
				lights_on = 1;
			}

			else {
				GPIO->OUTCLR = (1 << 13 | 1 << 14 | 1<< 15);
				lights_on = 0;
			}

		}

	}

	return 0;
}



ssize_t _write(int fd, const void *buf, size_t count) {
	char * letter = (char *)(buf);

	for(int i = 0; i < count; i++) {
		uart_send(*letter);
		letter++;
	}

	return count;
}