#include <msp430.h>
#include "adc.h"

/*
 * adc.c
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */

unsigned int *adc_data_pointer = (unsigned int*) 0;     //this points to the place we want adc data to be stored

void configure_ADC(void) {
    P1SEL |= BIT3;  //P1.3 is an input
    //Control Register 1
    //15-12: channel select, 1010 = temp sensor, select channel based on pin used
    //11-10: sample+hold source, 00 = ADC10SC bit
    //9: data format, 0 = straight binary
    //8: invert sample+hold, 0 = not inverted
    //7-5: clock divider, 0 = /1
    //4-3: clock source, 00 = ADC10OSC (~5MHz)
    //2-1: mode select, 00 = single-channel, single-conversion
    //0: busy flag, read this to see if it's in process
    ADC10CTL1 = 0b0000000000000000;

    //Control Register 0
    //15-13: reference select, 000 = VCC and VSS
    //12-11: sample+hold time, 10 = 16 clocks
    //10: sampling rate, 1 = 50ksps
    //9: reference output, 0 = off
    //8: reference buffer, 1 = only on during sample/conversion
    //7: multiple/sequence conversion, 0 = require rising edge of SHI
    //6: reference voltage, 0 = 1.5V
    //5: reference generator on/off, 0 = off
    //4: adc10 on/off, 0 = off (it may draw current if it's on, wait to set this until you're ready)
    //3: adc10 interrupt enable, 0 = disabled (we may want this enabled later?)
    //2: interrupt flag, 0 = do nothing with this
    //1: enable conversion, 0 = ADC disabled
    //0: START, 0 = stop (set this to 1 when you want to start the conversion)
    ADC10CTL0 = 0b0001010100000000;

    //input enable, A0 = bit0, A1 = bit1, etc. 1= enabled, 0 = disabled.
    //only enable the analog inputs we're using
    ADC10AE0 = 0;

#if defined(ADC10AE1)           //the MSP430F2272 has this, but the G2553 doesn't
    //input enable, 7-4: A15-A12, 3-0: reserved
    //only enable the analog inputs we're using
    ADC10AE1 = 0;
#endif
    //ADC10MEM; //this is where the conversion is stored
}

/* select_ADC_Channel(channel) puts the channel select bits into the ADC10CTL1 register
 *
 * pass in one of the defined ADC_CHANNEL_Ax's above, or ADC_CHANNEL(c) where c is a char like '0', 'E', etc.
 * also pass in one of the defined ADC_ENABLE_Ax's
 *
 * @param channel       the channel to select, note the format in the defines at the top of this file
 * @param enable_bit    the bit that will enable the corresponding analog pin (see defines above)
 */
unsigned char ADC_Select_And_Enable_Channel(unsigned int channel, unsigned char enable_bit) {
    if ((ADC10CTL1 & ADC10BUSY) == 0) {       //checks the ADC10CTL1 bit 0 to see if ADC is running
        ADC10CTL1   &= 0x0FFF;  //clear the channel select bits from this register
        ADC10CTL1   |= channel; //set the channel select bits using the shifted channel number
        ADC10AE0    &= 0x00;       //clear the enable bits (this may be bad, because it disables other ADCs)
#if defined(ADC10AE1)           //the MSP430F2272 has this, but the G2553 doesn't
        ADC10AE1 &= 0xFF;       //however, it may be good, because we could multiplex the pins
        if (channel > 7) {      //channels 12-15, and temp?
            ADC10AE1    |= enable_bit;
        } else
#endif
        {                //channels 0-7
            ADC10AE0    |= enable_bit;
        }
        return 1u;  //return success
    }
    return 0;   //return failure
}

unsigned char ADC_Enable(void) {
    if ((ADC10CTL1 & ADC10BUSY) == 0) {       //checks the ADC10CTL1 bit 0 to see if ADC is running
        ADC10CTL0 |= 0x0018;    //register bits: 4: ADC on/off, 3:ADC interrupt enable
        return 1u;              //return success
    }
    return 0u;                  //return failure
}

unsigned char ADC_Disable(void) {
    if ((ADC10CTL1 & ADC10BUSY) == 0) {       //checks the ADC10CTL1 bit 0 to see if ADC is running
        ADC10CTL0 &= 0xFFE7;    //register bits: 4: ADC on/off, 3:ADC interrupt enable
        return 1u;              //return success
    }
    return 0u;                  //return failure
}

/* ADC_Start(*storage location)
 *  points the ADC at some place in memory, then starts it
 *
 *  @param storage_location a pointer to the memory where adc results should be stored
 */
unsigned char ADC_Start(unsigned int *storage_location) {
    if ((ADC10CTL1 & ADC10BUSY) == 0) {       //checks the ADC10CTL1 bit 0 to see if ADC is running
        adc_data_pointer = storage_location; //point to some place in memory
        ADC10CTL0 |= 0x0003;    //register bits: 1: enable conversion, 0: start conversion
        return 1u;              //return success
    }
    return 0u;                  //return failure
}

//interrupt service routine for ADC
void __attribute__((interrupt(ADC10_VECTOR))) ADC_Interrupt(void) {
    *adc_data_pointer = ADC10MEM;   //this takes the ADC data and puts it in some memory location that adc_data_pointer is pointing at
}
