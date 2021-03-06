/**
 * @file    process_image.h
 * @brief   Prototype functions used in the image processing
 *
 */
 
#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

// C standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================================================================*/
/* Functions declaration.                                                    */
/*===========================================================================*/

uint16_t get_line_position(void);
void process_image_start(void);


/**
 * @brief			the following function sets the value of red_stop when
 * 					a red line has been found just after a new image is processed
 *
 * @param			image from the red channel of camera
 * 					image from the blue channel of camera
 * @return			none
 */
void check_red_stop(uint8_t *red_image,uint8_t *blue_image);

/**
 * @brief			the following functions manipulate the state of the boolean red_stop
 *
 *
 */
_Bool get_red_stop(void);
void reset_red_stop(void);
void set_red_stop(void);

/**
 * @brief			sets the value of the boolean horizontal_line when a black 
 * 					horizontal line has been processed, 
 * 					is used to know when to stop going around the obstacle
 *
 * @param 			image from the red channel of the camera
 *
 * @return			none
 */
void check_black_h_line(uint8_t *image_red,uint8_t *image_blue);


/**
 * @brief			the following functions manipulate the state of red_stop
 *
 *
*
 */
_Bool get_horizontal_line(void);
void reset_horizontal_line(void);
void set_horizontal_line(void);


#endif /* PROCESS_IMAGE_H */
