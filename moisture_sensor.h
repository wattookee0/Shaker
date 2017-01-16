/*
 * moisture_sensor.h
 *
 *  Created on: Jan 15, 2017
 *      Author: dmatthews
 */

#ifndef MOISTURE_SENSOR_H_
#define MOISTURE_SENSOR_H_

typedef struct {
    unsigned char   stale_value_flag : 1;   //0 = data is fresh, 1 = data is stale
    unsigned int    value;                  //the value retrieved from ADC
    unsigned char   input_designator;       //the pin identifier for setting ADC registers
    unsigned int    adc_channel;            //the channel identifier for setting ADC registers
} moisture_t;

unsigned char check_Moisture(void);
unsigned char get_Moisture_Value(moisture_t*);

#endif /* MOISTURE_SENSOR_H_ */
