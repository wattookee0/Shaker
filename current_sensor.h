/*
 * current_sensor.h
 *
 *  Created on: Mar 19, 2017
 *      Author: dmatthews
 */

#ifndef CURRENT_SENSOR_H_
#define CURRENT_SENSOR_H_

typedef struct {
    unsigned char   stale_value_flag : 1;   //0 = data is fresh, 1 = data is stale
    unsigned int    value;                  //the value retrieved from ADC
    unsigned char   input_designator;       //the pin identifier for setting ADC registers
    unsigned int    adc_channel;            //the channel identifier for setting ADC registers
} current_t;

unsigned char check_Current(void);
unsigned char get_Current_Value(current_t*);

#endif /* CURRENT_SENSOR_H_ */
