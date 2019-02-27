#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "accel.h"
#include "twi.h"
#include "ubit_led_matrix.h"
#include "utility.h"

#include "gpio.h"
#include "uart.h"

int main() {
    // WHO_AM_I
    uint8_t * data_buffer;
    data_buffer = (uint8_t *)malloc(8 * sizeof(uint8_t));
    uint8_t accelometer = 0x1D; // Adresse fra datablad
    uint8_t WHO_AM_I = 0xD; // Adresse fra datablad
    
    twi_init();
    
    twi_multi_read(accelometer, WHO_AM_I, 8, data_buffer); // Det skal leses 8 uint8_t
    
    
    
    
    
    
    
    return 0;
}

