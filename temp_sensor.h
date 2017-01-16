/*
 * temp_sensor.h
 *
 *  Created on: Jan 7, 2017
 *      Author: wattookee0
 */

#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

typedef struct {
    unsigned char stale_value_flag : 1; //indicates if the data captured has been check by the algorithm yet, 0 = no, 1 = yes
    unsigned int value;                 //the value captured by the ADC
    unsigned int trigger_threshold;     //should be set on system startup as the current ambient temp
    unsigned char input_designator;     //the pin identifier for setting ADC registers
    unsigned int adc_channel;           //the channel identifier for setting ADC registers
} temperature_t;

void calibrate_Temperature(void);
unsigned char check_Temperature(void);
unsigned char set_Temperature_Threshold(temperature_t*);
unsigned char get_Temperature_Value(temperature_t*);

#endif /* TEMP_SENSOR_H_ */
