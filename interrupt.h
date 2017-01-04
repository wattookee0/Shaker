/*
 * interrupt.h
 *
 *  Created on: Dec 31, 2016
 *      Author: dmatthews
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#define SPI_BUFFER_SIZE 20

extern unsigned char spi_input_buffer_index;
extern unsigned char spi_input_buffer[SPI_BUFFER_SIZE];

extern unsigned char spi_output_buffer_index;
extern unsigned char spi_output_buffer[SPI_BUFFER_SIZE];

#endif /* INTERRUPT_H_ */
