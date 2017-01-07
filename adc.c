#include <msp430.h>
#include "adc.h"

/*
 * adc.c
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */

void configure_ADC(void) {
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
    ADC10CTL0 = 0b0001010100000;
    //Control Register 1
    //15-12: channel select, 1010 = temp sensor, select channel based on pin used
    //11-10: sample+hold source, 00 = ADC10SC bit
    //9: data format, 0 = straight binary
    //8: invert sample+hold, 0 = not inverted
    //7-5: clock divider, 0 = /1
    //4-3: clock source, 00 = ADC10OSC (~5MHz)
    //2-1: mode select, 00 = single-channel, single-conversion
    //0: busy flag, read this to see if it's in process
    ADC10CTL1 = 0;
    //input enable, A0 = bit0, A1 = bit1, etc. 1= enabled, 0 = disabled.
    //only enable the analog inputs we're using
    ADC10AE0 = 0;
    //input enable, 7-4: A15-A12, 3-0: reserved
    //only enable the analog inputs we're using
    //ADC10AE1 = 0;
    //ADC10MEM; //this is where the conversion is stored

}


