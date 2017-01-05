#include "system_time.h"
/*
 * system_time.c
 *
 *  Created on: Jan 3, 2017
 *      Author: dmatthews
 */

unsigned long system_time = 0;  //start system time at 0

void configure_Main_Clock(void) {
    //Main oscillator setup
/*  RSELx = 14, DCOx = 3, MODx = 0 => Clock Freq = 8.6-13.9Mhz  */
    DCOCTL  = 0b01100000;   //7-5 = DCOx, 4-0 = MODx
    BCSCTL1 = 0b10001110;   //XT2 on/off, XTS mode, CLKA Divider (00), RSELx

    //Timer A setup
//TIMER A CONTROL: 15-10 not used, 9-8: source select (SMCLK), 7-6: divider
//5-4: mode control,  3 unused, 2 clear timer, 1 interrupt enable, 0 interrupt flag
    TACTL = 0x0000001000000010; //select SMCLK, divide by 1, stop mode, enable interrupt
//TIMER A CAPTURE CONTROL: 15-14: capture mode, 13-12 input select
//11 sync clk, 10 syncd input, 9 unused
//8 capture(0)/compare(1) mode, 7-5 output mode,
//4 cap/comp interrupt enable, 3 read CC input, 2 output low(0)/high(1)
//1 capture overflow flag, 0 cap/comp interrupt flag
    TACCTL = 0x0000100000010100;

}

unsigned char is_Timer_Expired(timer_t* timer_to_check) {
    if (timer_to_check->end_time > 0) {
        if (system_time > timer_to_check->end_time){
            return TRUE;
        } else {
            return FALSE;   //return timer running
        }
    }
    return -1u; //return failure (timer is off)
}
unsigned char is_Timer_Running(timer_t* timer_to_check) {
    if (timer_to_check->end_time > 0) {
        if (timer_to_check->end_time < system_time) {
            return TRUE;
        } else {
            return FALSE; //return failure (timer expired)
        }
    }
    return -1u; //return failure (timer is off)
}

void set_Timer_Interval(timer_t* timer_to_set, signed long new_interval) {
    timer_to_set->interval = new_interval;
}

void set_Timer_End_Time_And_Stop(timer_t* timer_to_set, signed long interval) {
    if (interval < 0) {
        interval *= -1;
    }
    signed long new_end_time = (system_time + interval) * -1;
    if (new_end_time > system_time) {
        timer_to_set->end_time = new_end_time;
        timer_to_set->interval = interval;
    }
}

void set_Timer_End_Time_And_Start(timer_t* timer_to_set, signed long interval) {
    if (interval < 0) {
        interval *= -1;
    }
    signed long new_end_time = (system_time+interval);
    if (new_end_time > system_time) {
        timer_to_set->end_time = new_end_time;
        timer_to_set->interval = interval;
    }
}

unsigned char start_Timer(timer_t* timer_to_start) {
    if (timer_to_start->end_time > system_time) {
        if (timer_to_start->end_time < 0) {
            timer_to_start->end_time *= -1;
            return 1u;  //return success
        } else {
            return 0u;  //return failure (timer already running)
        }
    }
    return -1u;         //return failure (timer expired)
}

unsigned char stop_Timer(timer_t* timer_to_stop) {
    if (timer_to_stop->end_time > system_time) {
        if (timer_to_stop->end_time > 0) {
            timer_to_stop->end_time *= -1;
            return 1u;  //return success
        } else {
            return 0u;  //return failure (timer already stopped)
        }
    }
    return -1u;         //return failure (timer expired)
}
