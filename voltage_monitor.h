/*
 * voltage_monitor.h
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */

#ifndef VOLTAGE_MONITOR_H_
#define VOLTAGE_MONITOR_H_

typedef struct {
    unsigned char stale_value_flag : 1;
    unsigned int value;
    unsigned char input_designator;
    unsigned int adc_channel;
} voltage_t;

unsigned char check_Battery_Voltage(void);
unsigned char check_Charger_Voltage(void);
unsigned char get_Voltage_Value(voltage_t*);

#endif /* VOLTAGE_MONITOR_H_ */
