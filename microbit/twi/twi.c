#include <stdint.h>
#include "twi.h"
#include "gpio.h"

#define TWI0 ((NRF_TWI_REG*)0x40003000)

typedef struct {
	// Tasks
	volatile uint32_t STARTRX;
	volatile uint32_t RESERVED0;
	volatile uint32_t STARTTX;
	volatile uint32_t RESERVED1[2];
	volatile uint32_t STOP;
	volatile uint32_t RESERVED2;
	volatile uint32_t SUSPEND;
	volatile uint32_t RESUME;
	// Events
	volatile uint32_t RESERVED3[56];
	volatile uint32_t STOPPED;
	volatile uint32_t RXDREADY;
	volatile uint32_t RESERVED4[4];
	volatile uint32_t TXDSENT;
	volatile uint32_t RESERVED5;
	volatile uint32_t ERROR;
	volatile uint32_t RESERVED6[4];
	volatile uint32_t BB;
	// Registers
	volatile uint32_t RESERVED7[49];
	volatile uint32_t SHORTS;
	volatile uint32_t RESERVED8[63];
	volatile uint32_t INTEN;
	volatile uint32_t INTENSET;
	volatile uint32_t INTENCLR;
	volatile uint32_t RESERVED9[110];
	volatile uint32_t ERRORSRC;
	volatile uint32_t RESERVED10[14];
	volatile uint32_t ENABLE;
	volatile uint32_t RESERVED11;
	volatile uint32_t PSELSCL;
	volatile uint32_t PSELSDA;
	volatile uint32_t RESERVED12[2];
	volatile uint32_t RXD;
	volatile uint32_t TXD;
	volatile uint32_t RESERVED13;
	volatile uint32_t FREQUENCY;
	volatile uint32_t RESERVED14[24];
	volatile uint32_t ADDRESS;
} NRF_TWI_REG;

void twi_init(){

    TWI0->PSELSCL = 0;
    TWI0->PSELSDA = 30;
    GPIO->PIN_CNF[0] = (3 << 2) | (6 << 8);
    GPIO->PIN_CNF[30] = (3 << 2) | (6 << 8);
    TWI0->RXDREADY = 0;
    TWI0->TXDSENT = 0;
    TWI0->FREQUENCY = 0x01980000;
    TWI0->ENABLE = 5;

}

void twi_multi_read(
		uint8_t slave_address,
		uint8_t start_register,
		int registers_to_read,
		uint8_t * data_buffer
		){

	int i;

    // Sett ADDRESS-registeret til slave_address
    TWI0->ADDRESS = slave_address;
    
    // Start en skriveoperasjon
    TWI0->STARTTX = 1;
    TWI0->TXDSENT = 0;
    
    // Overfor start_register til TWI-bussen
    TWI0->TXD = start_register;
    
    
    // Vent pa ACK
    while (!TWI0->TXDSENT) {}

    for (i = 0; i < 10; i++) {
        __asm("nop");
    }
    
    // Start leseoperasjon uten a stoppe bussen
    TWI0->RXDREADY = 0;
    TWI0->STARTRX = 1;
    
    // Les TWI-bussen (registers_to_read - 1) ganger
    for (i = 0; i < (registers_to_read - 1); i++) {
        while (!TWI0->RXDREADY) {} // Vent pa RXDREADY
        TWI0->RXDREADY = 0; // Alltid sett til null etter lesing
        data_buffer[i] = TWI0->RXD;
        
    }
    
    // Stopp
    TWI0->STOP = 1;
    
    // Les bussen en siste gang
    while(!TWI0->RXDREADY) {}
    data_buffer[registers_to_read - 1] = TWI0->RXD;
    TWI0->RXDREADY = 0;
    

}

void twi_multi_write(
		uint8_t slave_address,
		uint8_t start_register,
		int registers_to_write,
		uint8_t * data_buffer
		){

	int i;
	TWI0->ADDRESS = slave_address;
	TWI0->STARTTX = 1;

	TWI0->TXDSENT = 0;
	TWI0->TXD = start_register;
	while(!TWI0->TXDSENT);

	for(i = 0; i < registers_to_write; i++){
		TWI0->TXDSENT = 0;
		TWI0->TXD = data_buffer[i];
		while(!TWI0->TXDSENT);
	}

	TWI0->STOP = 0;
}
