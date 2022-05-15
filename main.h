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
