#include <msp430.h>

/*
 * clock.c
 *
 *  Created on: Dec 31, 2016
 *      Author: dmatthews
 */

void configure_Main_Clock(void) {
/*  RSELx = 14, DCOx = 3, MODx = 0 => Clock Freq = 8.6-13.9Mhz  */
    DCOCTL  = 0b01100000;   //7-5 = DCOx, 4-0 = MODx
    BCSCTL1 = 0b10001110;   //XT2 on/off, XTS mode, CLKA Divider (00), RSELx
}


