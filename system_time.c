#include <msp430.h>
#include "system_time.h"
/*
 * system_time.c
 *
 *  Created on: Jan 3, 2017
 *      Author: dmatthews
 */

#define SYSTEM_CLOCK_SPEED 1000000  //Hz
#define TIMER_A_INTERVAL 50000      //approx. 50ms

unsigned long system_time = 0;  //start system time at 0
unsigned char tick_tock = 0;    //this counts 0->4, then rolls over

void configure_Main_Clock(void) {
    //Main oscillator setup
/*  RSELx = 14, DCOx = 3, MODx = 0 => Clock Freq = 8.6-13.9Mhz  */
    //DCOCTL  = 0b01100000;   //7-5 = DCOx, 4-0 = MODx
    //BCSCTL1 = 0b10001110;   //XT2 on/off, XTS mode, CLKA Divider (00), RSELx

    DCOCTL = CALDCO_1MHZ;   //this is a calibrated setting supplied by the chip's hardcoded memory
    BCSCTL1 = CALBC1_1MHZ;  //so is this, this sets the main oscillator to 1MHz +/- 2%

/* I selected 1MHz because it supposedly only draws 300uA at full power,
 * If we assume that the clock runs at 1MHz then each clock cycle is 1us long
 * That means that if Timer A overflows at 0xFFFF (65535), then 65.535ms have passed
 * So, if we set TACCR0 = 50000, then every time the Timer_A interrupt hits, 50ms will have passed
 */

    //Timer A setup
//TIMER A CONTROL:
    //15-10 not used,
    //9-8: source select (SMCLK)
    //7-6: divider
    //5-4: mode control
    //3 unused
    //2 clear timer
    //1 interrupt enable,
    //0 interrupt flag
    TACTL = 0b0000001000010010; //select SMCLK, divide by 1, up mode, enable interrupt

    //TIMER A CAPTURE CONTROL:
    //15-14: capture mode,
    //13-12 input select
    //11 sync clk,
    //10 syncd input,
    //9 unused
    //8 capture(0)/compare(1) mode,
    //7-5 output mode,
    //4 cap/comp interrupt enable,
    //3 read CC input,
    //2 output low(0)/high(1)
    //1 capture overflow flag,
    //0 cap/comp interrupt flag
    TACCTL0 = 0b0000100000010100;

    //TACCR0, Timer A compare register, this is the value that is compared to the TAC register
    TACCR0 = TIMER_A_INTERVAL;
}

void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_Interrupt(void) {
    TACTL &=0b1111111111111110; //clear the interrupt flag
    if (tick_tock) {
        tick_tock = 0;
    } else {
        tick_tock = 1;
        system_time++;
    }
}

unsigned char is_Timer_Expired(timer_t* timer_to_check) {
    if (timer_to_check->overflow) { //if the timer setup resulted in an overflow
        if (timer_to_check->end_time > system_time){    //check if end_time > system_time
            timer_to_check->overflow = 0u;              //if >, sys_time has overflowed so reset the flag
        }                                               //and the timer is still running
        return FALSE;                                   //if <, sys_time has not overflowed, timer still running
    } else {                        //timer setup didn't overflow, or our last check reset the flag
        if (timer_to_check->end_time < system_time){    //system time has surpassed the threshold
            return TRUE;                                //so the time is expired
        } else {                                        //system time has not surpassed threshold
            return FALSE;                               //return timer running
        }
    }
}

unsigned long get_Timer_Elapsed_Time(timer_t* timer_to_peek) {
    //system time minus the timer's start time (end time - interval) is time elapsed since timer started
    unsigned long start_time = (timer_to_peek->end_time - timer_to_peek->interval);
    return (system_time - start_time);
}

void set_Timer_Interval(timer_t* timer_to_set, signed long new_interval) {
    timer_to_set->interval = new_interval;
}

void start_Timer(timer_t* timer_to_start) {
    unsigned long new_end_time = timer_to_start->interval + system_time;
    if (new_end_time < system_time) {   //if adding the interval overflowed system_time
        timer_to_start->overflow = 1u;  //set the flag
    }
    timer_to_start->end_time = new_end_time;
}

void stop_Timer(timer_t* timer_to_stop) {
    //just set end time to the current system time, which will make all is_Timer_Expired checks
    //evaluate to true
    timer_to_stop->end_time = system_time;
}
