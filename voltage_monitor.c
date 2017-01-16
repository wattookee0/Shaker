#include "main.h"
#include "voltage_monitor.h"
#include "adc.h"

/*
 * voltage_monitor.c
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */

#define LOW_BATTERY_VOLTAGE_THRESHOLD       (unsigned int)300u  //3V
#define MINIMUM_CHARGER_THRESHOLD           (unsigned int)360u  //3.6V
#define MAXIMUM_CHARGER_THRESHOLD           (unsigned int)420u  //4.2V, verify these are the correct numbers

                            //value, input designator, channel
voltage_t battery_voltage = {1, 0, ADC_ENABLE_A3, ADC_CHANNEL_A3};
voltage_t charger_voltage = {1, 0, 0, 0};

unsigned char check_Battery_Voltage(void) {
    if (battery_voltage.stale_value_flag == 1) {
        get_Voltage_Value(&battery_voltage);
    } else {
        if (battery_voltage.value < LOW_BATTERY_VOLTAGE_THRESHOLD) {
            system_status.sufficient_supply_voltage_flag = 0u;  //battery voltage is low
        } else {
            system_status.sufficient_supply_voltage_flag = 1u;  //battery voltage is good
        }
        battery_voltage.stale_value_flag = 1;  //data is now old
    }
    return 0u;
}

unsigned char check_Charger_Voltage(void) {
    if (charger_voltage.stale_value_flag == 1) {
        get_Voltage_Value(&charger_voltage);
    } else {
        if ((charger_voltage.value > MINIMUM_CHARGER_THRESHOLD)
           & (charger_voltage.value < MAXIMUM_CHARGER_THRESHOLD)) {
            system_status.charger_plugged_in_flag = 1u; //charger voltage is good
        } else {
            system_status.charger_plugged_in_flag = 0u; //charger voltage is bad or not present
        }
        charger_voltage.stale_value_flag = 1;  //data is now old
    }
    return 0u;
}

unsigned char get_Voltage_Value(voltage_t* source_to_check) {
    if (ADC_Select_And_Enable_Channel(source_to_check->adc_channel, source_to_check->input_designator) ) {
        if (ADC_Enable()) {
            ADC_Start(&(source_to_check->value));
            source_to_check->stale_value_flag = 0;  //let the algorithm know the data is fresh
            return 1u;  //return success
        }
        return 0u;
    }
    return 0u;
}

