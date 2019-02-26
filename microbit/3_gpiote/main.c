#include <stdint.h>
#include "gpiote.h"
#include "gpio.h"
#include "ppi.h"


int main () {

	//Config LED-matrix
	for (int i = 4; i < 16; i++) {
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	//Tasks
	GPIOTE->CONFIG[0] = (3 << 0) | (13 << 8) | (3 << 16);
	GPIOTE->CONFIG[1] = (3 << 0) | (14 << 8) | (3 << 16);
	GPIOTE->CONFIG[2] = (3 << 0) | (15 << 8) | (3 << 16);

	//Event
	GPIOTE->CONFIG[3] = (1 << 0) | (17 << 8) | (2 << 16);

	//Enable channels
	PPI->CHENSET = (1 << 0) | (1 << 1) | (1 << 2);

	//Channel 0
	PPI->PPI_CH[0].EEP = (uint32_t)&(GPIOTE->IN[3]);
	PPI->PPI_CH[0].TEP = (uint32_t)&(GPIOTE->OUT[0]);

	//Channel 1
	PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->IN[3]);
	PPI->PPI_CH[1].TEP = (uint32_t)&(GPIOTE->OUT[1]);

	//Channel 2
	PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->IN[3]);
	PPI->PPI_CH[2].TEP = (uint32_t)&(GPIOTE->OUT[2]);


	while (1) {
		int time = 1000;
		while (--time) {}
	}

	return 0;
}