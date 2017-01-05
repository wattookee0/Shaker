/*
 * system_time.h
 *
 *  Created on: Jan 3, 2017
 *      Author: dmatthews
 */

#ifndef SYSTEM_TIME_H_
#define SYSTEM_TIME_H_

#define FALSE   0u
#define TRUE    1u

typedef struct {
    signed long end_time;
    signed long interval;
} timer_t;

extern unsigned long system_time;

void configure_Main_Clock(void);

unsigned char is_Timer_Expired(timer_t*);
unsigned char is_Timer_Running(timer_t*);
void set_Timer_Interval(timer_t*, signed long);
void set_Timer_End_Time_And_Stop(timer_t*, signed long);
void set_Timer_End_Time_And_Start(timer_t*, signed long);
unsigned char start_Timer(timer_t*);
unsigned char stop_Timer(timer_t*);
#endif /* SYSTEM_TIME_H_ */
