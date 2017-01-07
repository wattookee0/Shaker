/*
 * adc.h
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */

#ifndef ADC_H_
#define ADC_H_

unsigned char   ADC_Select_And_Enable_Channel(unsigned int, unsigned char);
unsigned char   ADC_Enable(void);
unsigned char   ADC_Disable(void);
unsigned char   ADC_Start(unsigned int*);

#endif /* ADC_H_ */
