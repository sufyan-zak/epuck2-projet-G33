#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


#include <motors.h>
#include <audio/play_melody.h>

#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"

//constants for the different parts of the project
#define IMAGE_BUFFER_SIZE		640
#define WIDTH_SLOPE				10
#define MIN_LINE_WIDTH			40
#define ROTATION_COEFF			2.4

#define CITY_SIZE_LINE          4
#define NB_NODES  				16
#define MAX_PATH_SIZE           10

#define DEFAULT_SPEED_STEPS     500
#define DEFAULT_SPEED_CM        8


/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;

// starts all the peripherals used for the project
void peripherals_init(void);


void SendUint8ToComputer(uint8_t* data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif
