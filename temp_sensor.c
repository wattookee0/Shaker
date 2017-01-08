#include "temp_sensor.h"
#include "adc.h"

/*
 * temp_sensor.c
 *
 *  Created on: Jan 7, 2017
 *      Author: wattookee0
 */

temperature_t temperature = {0, ADC_ENABLE_TEMP, ADC_CHANNEL_TEMP_SENSOR};

void check_Temperature(void) {
    Temperature_Check(&temperature);
}

unsigned char Temperature_Check(temperature_t* temp_to_check) {
    if (ADC_Select_And_Enable_Channel(temp_to_check->adc_channel, temp_to_check->input_designator) ) {
        if (ADC_Enable()) {
            ADC_Start(&(temp_to_check->value));
            return 1u;  //return success
        }
        return 0u;
    }
    return 0u;
}
