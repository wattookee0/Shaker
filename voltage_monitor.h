/*
 * voltage_monitor.h
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */

#ifndef VOLTAGE_MONITOR_H_
#define VOLTAGE_MONITOR_H_

typedef struct {
    unsigned int value;
    unsigned char input_designator;
    unsigned int adc_channel;
} voltage_t;

void check_Battery_Voltage();
unsigned char Voltage_Check(voltage_t*);

#endif /* VOLTAGE_MONITOR_H_ */
