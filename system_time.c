#include "system_time.h"
/*
 * system_time.c
 *
 *  Created on: Jan 3, 2017
 *      Author: dmatthews
 */

unsigned long system_time = 0;  //start system time at 0

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
