/*
 * main.h
 *
 *  Created on: Dec 31, 2016
 *      Author: dmatthews
 */

#ifndef MAIN_H_
#define MAIN_H_

typedef struct {
    unsigned char charger_plugged_in_flag : 1;          //0 = no external voltage, 1 = external voltage present
    unsigned char sufficient_supply_voltage_flag : 1;   //0 = low battery, 1 = good battery voltage
    unsigned char moisture_present_flag : 1;            //0 = no moisture present, 1 = moisture present
    unsigned char good_temperature_flag : 1;            //0 = not over threshold, 1 = over threshold temperature
    unsigned char motor_timer_expired_flag : 1;         //0 = motor timer not expired, 1 = motor timer expired
    unsigned char motor_stalled_flag : 1;               //0 = motor not stalled, 1 = motor stalled
    unsigned char shake_detected_flag : 1;              //0 = no shake detected, 1 = shake detected
} system_status_t;

extern system_status_t system_status;

void system_Setup(void);    //holds functions that configure the hardware prior to the main loop

void update_Status_LEDs(void);  //used for demo, turns on/off LEDs based on system status struct
void check_Buttons(void);       //used for demo, checks the buttons and sets pins accordingly
void main_Charging(void);       //charging loop, manages power supply

#endif /* MAIN_H_ */
