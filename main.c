#include <msp430.h> 
#include "main.h"
#include "system_time.h"
#include "SPI.h"
#include "adc.h"
#include "voltage_monitor.h"
#include "temp_sensor.h"
#include "moisture_sensor.h"

/*
 * main.c
 */

#define BATTERY_VOLTAGE_SOURCE 3

unsigned int temp;
unsigned int temp1;

system_status_t system_status = {0, 0, 0, 0, 0, 0, 0}; //initialize system status register, holds flags for state of the system

void system_Setup(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    configure_Main_Clock();     //sets the main clock up to run about 10Mhz
    configure_SPI();            //sets the SPI registers
    configure_ADC();            //sets up the ADC registers

    P1DIR   |= 0b00000001;  //set P1.0 as output
    P1REN   |= 0b00000000;  //don't change these
    P1SEL   |= 0b00000000;
    P1SEL2  |= 0b00000000;
    P1OUT   |= 0b00000000;  //don't need to change this here

    P2DIR   |= 0b00001111;  //set P2.0, P2.1, P2.2, P2.3 as output
    P2REN   |= 0b00000000;  //don't change these
    P2SEL   |= 0b00000000;
    P2SEL2  |= 0b00000000;
    P2OUT   |= 0b00000011;  //don't need to change this here

    P1IE |= BIT3; // P1.3 interrupt enabled
    P1IFG &= ~BIT3; // P1.3 interrupt flag cleared

    __enable_interrupt();       //turn on all interrupts
    calibrate_Temperature();    //use ambient temp on power-on to set temp cutoff threshold
}

int main(void) {

    system_Setup();

    //MAIN LOOP
    while (1) {
        //check_Charger_Voltage();  //this is commented out because we haven't chosen the charger pin yet
        check_Battery_Voltage();
        check_Temperature();
        check_Moisture();
        //check temperature
        //check motor state/timer
        //check for accelerometer input
        //check_Buttons();
        update_Status_LEDs();
    }

    //SPI test below here
	//while (1) {
//	    temp = SPI_Word(0x2303);  //three pin sPI mode
/*        temp = SPI_Word(0xA000);  //read the CTRL1 register
	    temp = SPI_Word(0x203F);  //write: turn on active mode
	    temp = SPI_Word(0xA000);  //read the CTRL1 register
	    temp1 = SPI_Word(0x2201); //data ready interrupt enable
	    temp1 = SPI_Word(0b1000111101010101);
	    temp++;*/
	//}
}

void update_Status_LEDs(void) {
    if (system_status.moisture_present_flag == 1u) {
        P1OUT   |= 0b00000001;  //shine LED
    } else {
        P1OUT   &= 0b11111110;  //turn off LED
    }

    if (system_status.good_temperature_flag == 1u) {
        P2OUT   |= 0b00000001;  //shine LED
    } else {
        P2OUT   &= 0b11111110;  //turn off LED
    }

    if (system_status.sufficient_supply_voltage_flag == 1u) {
        P2OUT   |= 0b00000010;  //shine LED
    } else {
        P2OUT   &= 0b11111101;  //turn off LED
    }
}

void check_Buttons(void) {
    if ((P1IN & BIT3) == BIT3) {
        P2OUT   |= 0b00001100;
    } else {
        P2OUT   &= 0b11110011;
    }
}

void __attribute__((interrupt(PORT1_VECTOR))) P1_Interrupt(void) {
    if ((P2OUT & BIT2) == BIT2) {
        P2OUT &= ~BIT2;// Toggle P2.2
    } else {
        P2OUT |= BIT2;
    }
    P1IFG &= ~BIT3; // P1.3 interrupt flag cleared
}
