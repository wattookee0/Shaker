#include <msp430.h> 
#include "main.h"
#include "system_time.h"
#include "SPI.h"
#include "adc.h"
#include "voltage_monitor.h"
#include "temp_sensor.h"
#include "moisture_sensor.h"
#include "motor.h"
#include "accelerometer.h"
#include "current_sensor.h"

/*
 * main.c
 */

unsigned int temp;
unsigned int temp1;

system_status_t system_status = {0, 0, 0, 0, 0, 0, 0}; //initialize system status register, holds flags for state of the system

timer_t charge_timer = {0, 0, 0};


void system_Setup(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    configure_Main_Clock();     //sets the main clock up to run about 10Mhz
    configure_SPI();            //sets the SPI registers
    configure_ADC();            //sets up the ADC registers

    P1DIR   |= 0b00100001;  //set P1.0(Charger), 1.5 (LED) as output
    P1REN   |= 0b00000000;  //don't change these
    P1SEL   |= 0b00000000;
    //P1SEL2  |= 0b00000000;
    P1OUT   &= 0b00000000;  //don't need to change this here

    //literally nothing on P2
    P2DIR   |= 0b00000000;
    P2REN   |= 0b00000000;
    P2SEL   |= 0b00000000;
    //P2SEL2  |= 0b00000000;
    P2OUT   &= 0b00000000;

    P3DIR   |= 0b00011001;  //P3.0 out (SCLK), 3.3 out (CS), 3.4 out (SIMO), 3.5 in (SOMI), 3.7 in (ADC Vexternal)
    P3REN   |= 0b00000000;  //resistor enable (not needed)
    P3SEL   |= 0b00110001;  //P3.0 clock, 3.3 CS, 3.4 SIMO, 3.5 SOMI, these enable SPI on the SPI pins
    P3OUT   &= 0b00000000;

    P4DIR   |= 0b00000110;  //4.1 out (motor ctrl1), 4.2 out (motor ctrl2), 4.3 in (battery voltage ADC), 4.4 in (moisture ADC), 4.5 in (motor current ADC)
    P4REN   |= 0b00000000;  //none of these should be needed
    P4SEL   |= 0b00000000;
    P4OUT   &= 0b00000000;

    __enable_interrupt();       //turn on all interrupts
    calibrate_Temperature();    //use ambient temp on power-on to set temp cutoff threshold
    configure_Accel();          //configure the accelerometer
}

int main(void) {

    system_Setup();
    run_Motor_Timed(100);
    //MAIN LOOP
    while (1) {
        main_Charging();
        check_Temperature();
        check_Moisture();
        check_Current();
        check_Motor_State();
        poll_Accel();
        if (system_status.sufficient_supply_voltage_flag == TRUE) {
            if (system_status.good_temperature_flag == TRUE) {
                //if (system_status.moisture_present_flag == TRUE) {
                    if (system_status.motor_stalled_flag == FALSE) {
                        if (system_status.motor_timer_expired_flag == TRUE) {
                            if (system_status.shake_detected_flag == TRUE) {
                                run_Motor_Timed(100);
                            }
                        }
                    }
                //}
            }
        }
    }
}

void main_Charging(void) {
    unsigned char continue_charging = TRUE;
    unsigned int battery_voltage;
    while (continue_charging) {
        check_Charger_Voltage();    //see if charger is still plugged in
        check_Battery_Voltage();    //see if battery is charged
        battery_voltage = get_Battery_Voltage();//just pull the value from the struct
        continue_charging = FALSE;
        if (battery_voltage < BATTERY_CHARGED) {            //if battery needs charging
/*            if (system_status.charger_plugged_in_flag) {    //if a charger is plugged in
                continue_charging = TRUE;
                set_Timer_Interval(&charge_timer, 100);     //set to charge for 100 ticks
                start_Timer(&charge_timer);                 //start the timer
                while (!is_Timer_Expired(&charge_timer)) {  //if charge timer is running
                    P1OUT |= 0b00100001;                    //turn on charging circuit
                    //ONLY CHARGE, NO OTHER FUNCTIONS
                }
                P1OUT &= 0b11011110;                        //turn off charging circuit
            } else {                                        //no charger is plugged in
                P1OUT &= 0b11011110;                        //turn off charging circuit
                continue_charging = FALSE;                  //stop charging, escape loop
            }*/
        } else if (battery_voltage < BATTERY_DEAD) {
            battery_voltage++;              //do nothing, don't allow user to escape this loop, either
        } else {
            continue_charging = FALSE;                      //escape the loop (normal operation)
        }
    }
    P1OUT &= 0b11111110;    //turn off charging circuit
}

void __attribute__((interrupt(PORT1_VECTOR))) P1_Interrupt(void) {
    if ((P2OUT & BIT2) == BIT2) {
        P2OUT &= ~BIT2;// Toggle P2.2
    } else {
        P2OUT |= BIT2;
    }
    P1IFG &= ~BIT3; // P1.3 interrupt flag cleared
}
