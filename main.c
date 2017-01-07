#include <msp430.h> 
#include "main.h"
#include "system_time.h"
#include "SPI.h"
#include "adc.h"
#include "voltage_monitor.h"

/*
 * main.c
 */

#define BATTERY_VOLTAGE_SOURCE 3

unsigned int temp;
unsigned int temp1;

void system_Setup(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    configure_Main_Clock();     //sets the main clock up to run about 10Mhz
    configure_SPI();            //sets the SPI registers
    configure_ADC();            //sets up the ADC registers
    __enable_interrupt();      //turn on all interrupts
}

int main(void) {

    system_Setup();

    //MAIN LOOP
    while (1) {
        check_Battery_Voltage();
        //Check for presence of charger
        //check supply voltage
        //check for liquid
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
