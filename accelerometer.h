/*
 * accelerometer.h
 *
 *  Created on: Mar 18, 2017
 *      Author: dmatthews
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#define MAX_DATA 16

typedef struct {
    signed int axis_data[MAX_DATA];
    signed int average_value;
    signed long average_numerator;
    signed long average_denominator;
} axis_t;

typedef struct {
    axis_t x_axis;
    axis_t y_axis;
    axis_t z_axis;
    int data_index;
} accel_t;

void configure_Accel(void);
signed int poll_Accel(void);
void get_X_Value(accel_t*);
void get_Y_Value(accel_t*);
void get_Z_Value(accel_t*);
signed int abs(signed int);

#endif /* ACCELEROMETER_H_ */
