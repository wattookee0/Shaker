#include "voltage_monitor.h"
#include "adc.h"

/*
 * voltage_monitor.c
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */
                            //value, input pin, channel
voltage_t battery_voltage = {0, 0, 0};
voltage_t charger_voltage = {0, 0, 0};

unsigned char Voltage_Check(voltage_t* source_to_check) {
    if (ADC_Select_And_Enable_Channel(source_to_check->adc_channel, source_to_check->input_pin) ) {
        if (ADC_Enable()) {
            ADC_Start(&(source_to_check->value));
            return 1u;  //return success
        }
        return 0u;
    }
    return 0u;
}

