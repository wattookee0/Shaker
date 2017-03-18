#include "SPI.h"
#include "interrupt.h"
/*
 * SPI.c
 *
 *  Created on: Dec 31, 2016
 *      Author: dmatthews
 */

/* void configure_SPI()
 * sets up the hardware for SPI communication with the LIS2HH12
 *
 * best not to mess with this stuff.
 */
void configure_SPI(void) {
/*Reset/Configure procedure from user guide:
    1. Set UCA0CTL1.UCSWRST = 1;  (Holds module in reset)
    2. Configure all registers
    3. Configure the ports (UCA0 is on port 1)
    4. Clear UCA0CTL1.UCWRST = 0; (Releases module for operation)
    5. Enable interrupts if you need them (UCA0RXIE, UCA0TXIE)
*/
    //1. set UCSWRST = 1
    UCA0CTL1    =   0b11000001; //bit 0 is UCSWRST

    //2. configure the SPI control registers
                    //MSB->LSB
    UCA0CTL0    =   0b10101001; //clk phase select, clk polarity, MSB first, 8 bit length, master mode, 3 pin mode (00), sync mode
    UCA0CTL1    =   0b11000001; //clock source (00->11),  (5-1 unused), reset enable
    UCA0BR0     =   0b10000010; //this and next byte form baud rate prescaler (clock source/prescaler) (BR0 + BR1*256 = clock prescaler for baud rate)
    UCA0BR1     =   0b00000000;
    UCA0STAT    =   0b00000000; //listen enable, framing error flag, overrun error flag, parity error flag, (4-1 not used on SPI), USCI busy flag
    //UCA0RXBUF   =   0b00000000; //receive buffer, reading this resets all the flags
    //UCA0TXBUF   =   0b00000000; //transmit buffer, writing to this clears the tx interrupt flag

    //3. configure the port
        //this is from G2553 launchpad
        //P3OUT   |= 0b00110100;
        //P3DIR   |= 0b00110100; //Pin 5: slave enable (std I/O), 4: clockout, 2: master out, 1: master in, others: whatever
        //P3REN   |= 0b00000000;
        //P3SEL   |= 0b00010110;
        //P1SEL2  |= 0b00010110;
        //P3OUT   |= 0b00100000;  //go ahead and set CS high (which un-selects the chip)

    P3OUT   |= 0b00011001;
    P3DIR   |= 0b00011001;  //P3.0 out (SCLK), 3.3 out (CS), 3.4 out (SIMO), 3.5 in (SOMI), 3.7 in (ADC Vexternal)
    P3REN   |= 0b00000000;  //resistor enable (not needed)
    P3SEL   |= 0b00110001;  //P3.0 clock, 3.3 CS, 3.4 SIMO, 3.5 SOMI, these enable SPI on the SPI pins
    P3OUT   |= 0b00001000;

    //4. enable SPI
    UCA0CTL1    =   0b11000000; //bit 0 is UCSWRST

    //5. interrupt enable register
    //we're not using the interrupt, we're using busy loops instead
    //you should use the interrupt, but for our application that's probably too involved and unnecessary
}

/* unsigned char SPI_Byte(unsigned char byte_to_send)
 * Sends/receives a single byte via SPI
 *
 * @param byte_to_send  the byte to send while receiving
 * @returns received byte
 */
unsigned char SPI_Byte(unsigned char byte_to_send) {
    IFG2 &= ~(UCA0RXIFG);
    P3OUT &= 0b11110111;
    UCA0TXBUF = byte_to_send;
    while (!(IFG2 & UCA0RXIFG));
    P3OUT |= 0b00001000;
    return (UCA0RXBUF);
}

/* unsigned int SPI_Word(unsigned int word_to_send)
 * Sends/receives an entire word (two bytes) via SPI
 *
 * @param word_to_send  the word to send via SPI
 * @returns received_word   the word received via SPI
 */
unsigned int SPI_Word(unsigned int word_to_send) {
    unsigned int word_received = 0;
    unsigned char low_byte_received;
    unsigned char high_byte_received;
    unsigned char low_byte_to_send = word_to_send;              //grab the lower 8 bits
    unsigned char high_byte_to_send = (word_to_send >> 8);      //grab the upper 8 bit
    IFG2 &= ~(UCA0RXIFG);                                       //clear the flag
    P3OUT &= 0b11110111;                                        //turn chip select on
    while (!(IFG2 & UCA0TXIFG));                                //wait for transmit buffer to be ready
    UCA0TXBUF = high_byte_to_send;                              //send the high 8
    while (!(IFG2 & UCA0RXIFG));                                //wait for RX to finish
    high_byte_received = UCA0RXBUF;                             //grab the RX'd byte
    UCA0TXBUF = low_byte_to_send;                               //send the low 8
    while (!(IFG2 & UCA0RXIFG));                                //wait for RX to finish
    low_byte_received = UCA0RXBUF;                              //grab the RX'd byte
    P3OUT |= 0b00001000;                                        //turn off chip select
    word_received = high_byte_received;                         //now put the two bytes into a word
    word_received = (word_received << 8);
    word_received += low_byte_received;
    return (word_received);                                     //send the word back
}
