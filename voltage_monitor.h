/*
 * voltage_monitor.h
 *
 *  Created on: Jan 4, 2017
 *      Author: dmatthews
 */

#ifndef VOLTAGE_MONITOR_H_
#define VOLTAGE_MONITOR_H_

#define LOW_BATTERY_VOLTAGE_THRESHOLD       (unsigned int)280u  //3V
#define MINIMUM_CHARGER_THRESHOLD           (unsigned int)300u  //830u  //3.6V //600 is just for testing
#define MAXIMUM_CHARGER_THRESHOLD           (unsigned int)980u  //4.2V, verify these are the correct numbers

#define BATTERY_CHARGED 575u
#define BATTERY_DEAD    525u

typedef struct {
    unsigned char stale_value_flag : 1;
    unsigned int value;
    unsigned char input_designator;
    unsigned int adc_channel;
} voltage_t;

unsigned char check_Battery_Voltage(void);
unsigned char check_Charger_Voltage(void);
unsigned char get_Voltage_Value(voltage_t*);

unsigned int get_Battery_Voltage(void);
unsigned int get_Charger_Voltage(void);

#endif /* VOLTAGE_MONITOR_H_ */
