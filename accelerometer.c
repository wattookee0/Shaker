/*
 * accelerometer.c
 *
 *  Created on: Mar 18, 2017
 *      Author: dmatthews
 */
#include "main.h"
#include "accelerometer.h"
#include "SPI.h"

accel_t accel = {{0},{0},{0}};

void configure_Accel(void) {
    unsigned int temp;
    temp = SPI_Word(0x2302);  //CTRL4: disable I2C
    temp = SPI_Word(0xA300);  //read the CTRL4 register
    if (temp == 0x0002) {
        P1OUT |= 0b00100000;    //flash LED to indicate success
    }
    temp = SPI_Word(0x201F);  //10Hz data rate, enable all 3 axes and BDU
    temp = SPI_Word(0xA000);  //read the CTRL1 register
    if (temp == 0x001F) {
        P1OUT &= 0b11011111;    //turn off LED to indicate success
    }
}

signed int poll_Accel(void) {
    int status = 0;
    int temp = 0, temp1 = 0;;
    status = SPI_Word(0xA700);    //read status register
    if (status & 0b00001000) {    //bit 3 is "new data available" bit, if it's set
        get_X_Value(&accel);
        get_Y_Value(&accel);
        //get_Z_Value(&accel);
        if (accel.data_index < MAX_DATA - 1) {        //wrap data around
            accel.data_index++;
        } else {
            accel.data_index = 0;
        }
    }
    temp = (accel.y_axis.axis_data[accel.data_index-1] + accel.y_axis.axis_data[accel.data_index-2])/2;
    temp1 = (accel.x_axis.axis_data[accel.data_index-1] + accel.x_axis.axis_data[accel.data_index-2])/2;
    if (abs(temp) > abs(accel.y_axis.average_value)*1.5) {
        if (abs(temp1) > abs(accel.x_axis.average_value)*1.5) {
            system_status.shake_detected_flag = 1u;
        } else {
            system_status.shake_detected_flag = 0u;
        }
    } else {
        system_status.shake_detected_flag = 0u;
    }
    return 0;
}

void get_X_Value(accel_t* accel) {
    signed int x = 0;
    x |= SPI_Word(0xA800);          //get X-axis low byte
    x |= (SPI_Word(0xA900) << 8);   //get high byte
    if (accel->x_axis.average_denominator < MAX_DATA) {
        accel->x_axis.average_denominator++;
    } else {
        accel->x_axis.average_numerator -= accel->x_axis.axis_data[accel->data_index];
    }
    accel->x_axis.axis_data[accel->data_index] = x;
    accel->x_axis.average_numerator += x;
    accel->x_axis.average_value = accel->x_axis.average_numerator/accel->x_axis.average_denominator;
}

void get_Y_Value(accel_t* accel) {
    signed int y = 0;
    y |= SPI_Word(0xAA00);          //get Y-axis low byte
    y |= (SPI_Word(0xAB00) << 8);   //get high byte
    if (accel->y_axis.average_denominator < MAX_DATA) {
        accel->y_axis.average_denominator++;
    } else {
        accel->y_axis.average_numerator -= accel->y_axis.axis_data[accel->data_index];
    }
    accel->y_axis.axis_data[accel->data_index] = y;
    accel->y_axis.average_numerator += y;
    accel->y_axis.average_value = accel->y_axis.average_numerator/accel->y_axis.average_denominator;
}

void get_Z_Value(accel_t* accel) {
    signed int z = 0;
    z |= SPI_Word(0xAC00);          //get Z-axis low byte
    z |= (SPI_Word(0xAD00) << 8);   //get high byte
    if (accel->z_axis.average_denominator < MAX_DATA) {
        accel->z_axis.average_denominator++;
    } else {
        accel->z_axis.average_numerator -= accel->z_axis.axis_data[accel->data_index];
    }
    accel->z_axis.axis_data[accel->data_index] = z;
    accel->z_axis.average_numerator += z;
    accel->z_axis.average_value = accel->z_axis.average_numerator/accel->z_axis.average_denominator;
}

signed int abs(signed int value) {
    if (value < 0) {
        return value * -1;
    }
    return value;
}
