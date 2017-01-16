#include "main.h"
#include "temp_sensor.h"
#include "adc.h"

/*
 * temp_sensor.c
 *
 *  Created on: Jan 7, 2017
 *      Author: dmatthews
 */
                            //staleness flag, value, threshold value, input pin, adc channel
temperature_t temperature = {1, 0, 0, ADC_ENABLE_TEMP, ADC_CHANNEL_TEMP_SENSOR};
                            //start the staleness flag at 1 so that the algorithm will update it on the first go round
unsigned char check_Temperature(void) {
    if (temperature.stale_value_flag == 1) {    //if we've seen the data in the struct already
        get_Temperature_Value(&temperature);    //get new data
    } else {                                    //otherwise, process the data we haven't seen yet
        if (temperature.value > 2*temperature.trigger_threshold) {
            system_status.over_temperature_flag = 1u;   //set the over temperature flag
        } else {
            system_status.over_temperature_flag = 0u;   //clear the over temperature flag
        }
        temperature.stale_value_flag = 1;       //let the algorithm know we've seen the current data
    }
    return 0u;
}

void calibrate_Temperature(void) {
    set_Temperature_Threshold(&temperature);
}

unsigned char set_Temperature_Threshold(temperature_t* temp_to_set) {
    if (ADC_Select_And_Enable_Channel(temp_to_set->adc_channel, temp_to_set->input_designator) ) {
        if (ADC_Enable()) {
            ADC_Start(&(temp_to_set->trigger_threshold));
            return 1u;  //return success
        }
        return 0u;
    }
    return 0u;
}

unsigned char get_Temperature_Value(temperature_t* temp_to_get) {
    if (ADC_Select_And_Enable_Channel(temp_to_get->adc_channel, temp_to_get->input_designator) ) {
        if (ADC_Enable()) {
            ADC_Start(&(temp_to_get->value));
            temp_to_get->stale_value_flag = 0;  //reset the stale value flag so algorithm knows data is new
            return 1u;  //return success
        }
        return 0u;
    }
    return 0u;
}
