/*
 * SPI.h
 *
 *  Created on: Dec 31, 2016
 *      Author: wattookee0
 */

#ifndef SPI_H_
#define SPI_H_

#include "msp430g2553.h"

void configure_SPI(void);  //sets up hardware for SPI communication
void transmit_SPI(void);    //triggers hardware to send a loaded byte
unsigned char SPI_Byte(unsigned char byte_to_send); //sends/receives a byte (8 bits)
signed int SPI_Word(unsigned int word_to_send);  //sends/receives a word (16 bits)

#endif /* SPI_H_ */
