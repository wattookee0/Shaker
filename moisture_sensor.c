#include "main.h"
#include "moisture_sensor.h"
#include "adc.h"

/*
 * moisture_sensor.c
 *
 *  Created on: Jan 15, 2017
 *      Author: dmatthews
 */
#define MINIMUM_MOISTURE_THRESHOLD (unsigned int)600u   //divider is R1=R2=1MOhm
                                                        //so ADC should read 512 (VCC/2) when no moisture
                                                        //and the water contacts are in parallel with R1
                                                        //so ADC should read >512 when yes moisture
                                                        //finally, the value bounces all over
                                                        //so I added in some "buffer" space to the min value

                        //stale value flag, value, pin designator, adc channel
moisture_t moisture = {1, 0, ADC_ENABLE_A3, ADC_CHANNEL_A3};
                    //start with stale value flag high so the algorithm updates immediately

unsigned char check_Moisture(void) {
    if (moisture.stale_value_flag == 1) {    //if we've seen the data in the struct already
            get_Moisture_Value(&moisture);    //get new data
        } else {                                    //otherwise, process the data we haven't seen yet
            if (moisture.value > MINIMUM_MOISTURE_THRESHOLD) {
                system_status.moisture_present_flag = 1u;   //set the over temperature flag
            } else {
                system_status.moisture_present_flag = 0u;   //clear the over temperature flag
            }
            moisture.stale_value_flag = 1;       //let the algorithm know we've seen the current data
        }
        return 0u;
}

unsigned char get_Moisture_Value(moisture_t* moisture_to_get) {
    if (ADC_Select_And_Enable_Channel(moisture_to_get->adc_channel, moisture_to_get->input_designator) ) {
        if (ADC_Enable()) {
            ADC_Start(&(moisture_to_get->value));
            moisture_to_get->stale_value_flag = 0;  //clear flag so algorithm knows data is fresh
            return 1u;  //return success
        }
        return 0u;
    }
    return 0u;
}
