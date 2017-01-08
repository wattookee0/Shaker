/*
 * temp_sensor.h
 *
 *  Created on: Jan 7, 2017
 *      Author: wattookee0
 */

#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

typedef struct {
    unsigned int value;
    unsigned char input_designator;
    unsigned int adc_channel;
} temperature_t;

void check_Temperature(void);
unsigned char Temperature_Check(temperature_t*);

#endif /* TEMP_SENSOR_H_ */
