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
    unsigned long end_time;
    unsigned long interval;
    unsigned char overflow;
} timer_t;

extern unsigned long system_time;

void configure_Main_Clock(void);

unsigned char is_Timer_Expired(timer_t*);
unsigned long get_Timer_Elapsed_Time(timer_t*);
void set_Timer_Interval(timer_t*, signed long);
void start_Timer(timer_t*);
void stop_Timer(timer_t*);
#endif /* SYSTEM_TIME_H_ */
