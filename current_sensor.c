/*
 * current_sensor.c
 *
 *  Created on: Mar 19, 2017
 *      Author: dmatthews
 */
#include "main.h"
#include "current_sensor.h"
#include "adc.h"

#define MAXIMUM_CURRENT_THRESHOLD (unsigned int)300u

                        //stale value flag, value, pin designator, adc channel
current_t current = {1, 0, ADC_ENABLE_A14, ADC_CHANNEL_A14};    //P1.6
                    //start with stale value flag high so the algorithm updates immediately

unsigned char check_Current(void) {
    if (current.stale_value_flag == 1) {    //if we've seen the data in the struct already
            get_Current_Value(&current);    //get new data
        } else {                                    //otherwise, process the data we haven't seen yet
            if (current.value > MAXIMUM_CURRENT_THRESHOLD) {
                system_status.motor_stalled_flag = 1u;   //set the moisture present flag
            } else {
                system_status.motor_stalled_flag = 0u;   //clear the moisture present flag
            }
            current.stale_value_flag = 1;       //let the algorithm know we've seen the current data
        }
        return 0u;
}

unsigned char get_Current_Value(current_t* current_to_get) {
    if (ADC_Select_And_Enable_Channel(current_to_get->adc_channel, current_to_get->input_designator) ) {
        if (ADC_Enable()) {
            ADC_Start(&(current_to_get->value));
            current_to_get->stale_value_flag = 0;  //clear flag so algorithm knows data is fresh
            return 1u;  //return success
        }
        return 0u;
    }
    return 0u;
}


