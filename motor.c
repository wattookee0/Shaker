/*
 * motor.c
 *
 *  Created on: Mar 17, 2017
 *      Author: dmatthews
 */
#include "main.h"
#include "motor.h"

#define MOTOR_ON    1u
#define MOTOR_OFF   0u

#define MOTOR_LEFT  1u
#define MOTOR_RIGHT 0u

#define MOTOR_GO_RIGHT  P4OUT   |= 0b00000100
#define MOTOR_GO_LEFT   P4OUT   |= 0b00000010
#define MOTOR_STOP       P4OUT   &= 0b11111001

motor_t motor = {{0, 100000, 0},0,0};   //motor_timer, motor status = off, motor dir = right

void run_Motor_Timed(unsigned long interval) {
    if (motor.motor_status == MOTOR_OFF) {
        set_Timer_Interval(&(motor.motor_timer), interval);
        start_Timer(&(motor.motor_timer));
        system_status.motor_timer_expired_flag = FALSE;
        MOTOR_GO_RIGHT;
        motor.motor_status = MOTOR_ON;
        motor.motor_direction = MOTOR_RIGHT;
    }
}

void check_Motor_State(void) {
    if (motor.motor_status == MOTOR_ON) {
        //stop the motor if its timer is expired or current indicates stall
        if (is_Timer_Expired(&(motor.motor_timer)) || (system_status.motor_stalled_flag == TRUE)) {   //true = it's expired
            if (get_Timer_Elapsed_Time(&(motor.motor_timer)) > 3) {
                MOTOR_STOP;
                motor.motor_status = MOTOR_OFF;
                stop_Timer(&(motor.motor_timer));
                system_status.motor_timer_expired_flag = TRUE;
            }
        }
    }
}



