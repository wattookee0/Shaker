#include <msp430.h> 
#include "main.h"
#include "system_time.h"
#include "SPI.h"
#include "adc.h"
#include "voltage_monitor.h"
#include "temp_sensor.h"

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
    __enable_interrupt();       //turn on all interrupts
    calibrate_Temperature();    //use ambient temp on power-on to set temp cutoff threshold
}

int main(void) {

    system_Setup();

    //MAIN LOOP
    while (1) {
        //check_Charger_Voltage();  //this is commented out because we haven't chosen the charger pin yet
        //check_Battery_Voltage();
        check_Temperature();
        check_Moisture();
        //check temperature
        //check motor state/timer
        //check for accelerometer input
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
