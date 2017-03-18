/*
 * motor.h
 *
 *  Created on: Mar 17, 2017
 *      Author: dmatthews
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <msp430.h>
#include "system_time.h"

typedef struct {
    timer_t motor_timer;            //times movements
    unsigned char motor_status;     //1 = on, 0 = off
    unsigned char motor_direction;  //1 = left, 0 = right
} motor_t;

void run_Motor_Timed(unsigned long);
void check_Motor_State(void);

#endif /* MOTOR_H_ */
