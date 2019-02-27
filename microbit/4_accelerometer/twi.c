#include "twi.h"
#include "gpio.h"


void twi_init() {
    TWIO->PSELSCL = 0;
    TWIO->PSELSDA = 30;
    GPIO->PIN_CNF[0] = (1 << 2) | (1 << 10);
    GPIO->PIN_CNF[30] = (1 << 2) | (1 << 10); //Stemmer dette?
    TWIO->RXDREADY = 0;
    TWIO->TXDSENT = 0;
    TWIO->FREQUENCY = 0x01980000;
    TWIO->ENABLE = 5;
}

void twi_multi_read(uint8_t slave_address,
                    uint8_t start_register,
                    int registers_to_read,
                    uint8_t * data_buffer) {
    
    // Sett ADDRESS-registeret til slave_address
    TWIO->ADDRESS = slave_address;
    
    // Start en skriveoperasjon
    TWIO->STARTTX = 1;
    TWIO->TXDSENT = 0;
    
    // Overfor start_register til TWI-bussen
    TWIO->TXD = start_register;
    
    
    // Vent pa ACK
    while (!TWIO->TXDSENT) {}
    
    // Start leseoperasjon uten a stoppe bussen
    TWIO->RXRDEADY = 0;
    TWIO->STARTRX = 1;
    
    // Les TWI-bussen (registers_to_read - 1) ganger
    for (int i = 0; i < (registers_to_read - 1); i++) {
        while (!TWIO->RXDREADY) {} // Vent pa RXDREADY
        data_buffer[i] = TWIO->RXD;
        TWIO->RXDREADY = 0; // Alltid sett til null etter lesing
    }
    
    // Stopp
    TWIO->STOP = 1;
    
    // Les bussen en siste gang
    while(!TWIO->RXDREADY) {}
    data_buffer[registers_to_read - 1] = TWIO->RXD;
    TWIO->RXDREADY = 0;
    TWIO->STARTRX = 1;
}
