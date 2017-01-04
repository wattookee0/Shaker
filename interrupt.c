#include <msp430.h>
#include "interrupt.h"
#include "system_time.h"
/*
 * interrupt.c
 *
 *  Created on: Dec 31, 2016
 *      Author: dmatthews
 */

unsigned char spi_input_buffer_index = 0;
unsigned char spi_input_buffer[SPI_BUFFER_SIZE];

unsigned char spi_output_buffer_index = 0;
unsigned char spi_output_buffer[SPI_BUFFER_SIZE];

void __attribute__((interrupt(TIMER1_A0_VECTOR))) Timer1_A0_Interrupt(void) {
    system_time++;
}

void __attribute__((interrupt(USCIAB0RX_VECTOR))) SPI_RX_Interrupt(void) {
    spi_input_buffer[spi_input_buffer_index] = UCA0RXBUF;   //grab any data
    if (spi_input_buffer_index < SPI_BUFFER_SIZE) {
        spi_input_buffer_index++;
    } else {
        spi_input_buffer_index = 0;
    }
}

void __attribute__((interrupt(USCIAB0TX_VECTOR))) SPI_TX_Interrupt(void) {
    UCA0TXBUF = spi_output_buffer[spi_output_buffer_index]; //grab any data
    if (spi_output_buffer_index < SPI_BUFFER_SIZE) {
        spi_output_buffer_index++;
    } else {
        spi_output_buffer_index = 0;
    }
    IE2 = 0b00000001; //(7-4 not used), UCB0TX, UCB0RX, UCA0TX, UCA0RX
}
