/**
 * @file    main.c
 * @brief   This is the main file that controls other modules and initializes peripherals
 */

// standard C headers
#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// e-puck2 main processor headers
#include <motors_custom.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <sensors/proximity.h>
#include <camera/po8030.h>
#include <chprintf.h>
#include <spi_comm.h>

// module headers
#include <process_image.h>
#include <path_regulator.h>


messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

void SendUint8ToComputer(uint8_t* data, uint16_t size) 
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, size);
}

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

int main(void)
{
	peripherals_init();
	//stars the threads for the pi regulator and the processing of the image
	process_image_start();
	path_regulator_start();
	leds_toggle_start();

    // Infinite loop.
    while (1) {

        chThdSleepMilliseconds(100);
    }
	return 0;
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void peripherals_init(void){

    halInit();
    chSysInit();

    //starts the serial communication
    serial_start();

    //start the USB communication
    usb_start();

    //starts the camera
    dcmi_start();
	po8030_start();

	//inits the motors
	motors_init();
    spi_comm_start();
	//inits IR and ToF sensors
    messagebus_init(&bus,&bus_lock,&bus_condvar);
	proximity_start();
	VL53L0X_start();

}
void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
