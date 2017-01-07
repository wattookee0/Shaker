/*
 * adc.h
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */

#ifndef ADC_H_
#define ADC_H_

#define ADC_CHANNEL(c)              0xc000

#define ADC_CHANNEL_A0              0x0000
#define ADC_CHANNEL_A1              0x1000
#define ADC_CHANNEL_A2              0x2000
#define ADC_CHANNEL_A3              0x3000
#define ADC_CHANNEL_A4              0x4000
#define ADC_CHANNEL_A5              0x5000
#define ADC_CHANNEL_A6              0x6000
#define ADC_CHANNEL_A7              0x7000
//unused
#define ADC_CHANNEL_TEMP_SENSOR     0xA000
//unused
#define ADC_CHANNEL_A12             0xC000
#define ADC_CHANNEL_A13             0xD000
#define ADC_CHANNEL_A14             0xE000
#define ADC_CHANNEL_A15             0xF000

//these select 1 bit of the ADC10AE0/ADC10AE1 registers, which enable the corresponding analog pins
#define ADC_ENABLE_A0       0x01
#define ADC_ENABLE_A1       0x02
#define ADC_ENABLE_A2       0x04
#define ADC_ENABLE_A3       0x08
#define ADC_ENABLE_A4       0x10
#define ADC_ENABLE_A5       0x20
#define ADC_ENABLE_A6       0x40
#define ADC_ENABLE_A7       0x80
//unused
#define ADC_ENABLE_TEMP     0x04    //this is a reserved bit in the register, not sure if this works?
//unused
#define ADC_ENABLE_A12      0x10
#define ADC_ENABLE_A13      0x20
#define ADC_ENABLE_A14      0x40
#define ADC_ENABLE_A15      0x80

void configure_ADC(void);
unsigned char   ADC_Select_And_Enable_Channel(unsigned int, unsigned char);
unsigned char   ADC_Enable(void);
unsigned char   ADC_Disable(void);
unsigned char   ADC_Start(unsigned int*);

#endif /* ADC_H_ */
