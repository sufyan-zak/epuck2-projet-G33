/**
 * @file    process_image.c
 * @brief   Processes the image seen by the robot , changes
 * 			the booleans red_stop and horizontal_line when a stop or the
 * 			end of an obstacle is found ando also spots the
 *
 *
 *
 */


//standard C headers
#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

//e-puck2 main processor headers
#include <camera/po8030.h>
#include <constants.h>

//module headers
#include <process_image.h>
#include <main.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
#define BLACK_H_LINE_AVERAGE 25
#define BLUE_LINE_AVERAGE 60
#define COEFF_MEAN_RED 0.8

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static uint16_t line_position = IMAGE_BUFFER_SIZE/2;	//middle of the array
static uint16_t line_width = 0;
static uint16_t begin;
static uint16_t  end;
static _Bool horizontal_line = 0;
static _Bool red_stop = 0;
//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

/*===========================================================================*/
/* Module threads.	                                                         */
/*===========================================================================*/

static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//takes pixels 0 to IMAGE_BUFFER_SIZE of the line 200 + 201 (minimum 2 lines because reasons)
	po8030_advanced_config(FORMAT_RGB565, 0, 200, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();

    while(1){
        //starts a capture
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);
    }
}


static THD_WORKING_AREA(waProcessImage, 4096);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
	uint8_t image_red[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_blue[IMAGE_BUFFER_SIZE] = {0};
	bool send_to_computer = true;

    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);

		//gets the pointer to the array filled with the last image in RGB565
		img_buff_ptr = dcmi_get_last_image_ptr();

		//extracts the red  and the blue
		for(uint16_t i = 0 ; i <  IMAGE_BUFFER_SIZE ; i++){
			//extracts last 5bits of the second byte for the blue pixels
			//takes nothing from the first byte
			image_blue[i] = (img_buff_ptr[2*i+1]&0x1F) << 3;

			//extracts first 5bits of the first byte for the red pixels
			//takes nothing from the second byte
			image_red[i] = (uint8_t)img_buff_ptr[2*i]&0xF8;
		}

		//sets the value of "red_stop" if a red line is found
		check_red_stop(image_red,image_blue);


		//sets the value of "horizontal_line" if a black horizontal line is found
		check_black_h_line(image_red,image_blue);


		if(send_to_computer){

			//sends to the computer the image
			//SendUint8ToComputer(image_red, IMAGE_BUFFER_SIZE);
		}

		//invert the bool
		send_to_computer = !send_to_computer;
    }
}

/*===========================================================================*/
/* Module functions.	                                                     */
/*===========================================================================*/

/** @brief	Uses the buffer computed by the image processing to extract the first and
 * 			the last pixels of the black line used after to spot the red line
 * 			Also extracts the middle position of the line to the variable "line_position"
 *
 *  @return	none
 *
 */
void extract_line(uint8_t *buffer){

	uint16_t i = 0;
	uint8_t stop = 0, wrong_line = 0, line_not_found = 0;
	uint32_t mean = 0;

	static uint16_t last_width =0;

	//performs an average
	for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){
		mean += buffer[i];
	}
	mean /= IMAGE_BUFFER_SIZE;

	do{
		wrong_line = 0;
		//search for a begin
		while(stop == 0 && i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE))
		{ 
			//the slope must at least be WIDTH_SLOPE wide and is compared
		    //to the mean of the image
		    if(buffer[i] > mean && buffer[i+WIDTH_SLOPE] < mean)
		    {
		        begin = i;
		        stop = 1;
		    }
		    i++;
		}
		//if a begin was found, search for an end
		if (i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE) && begin)
		{
		    stop = 0;
		    
		    while(stop == 0 && i < IMAGE_BUFFER_SIZE)
		    {
		        if(buffer[i] > mean && buffer[i-WIDTH_SLOPE] < mean)
		        {
		            end = i;
		            stop = 1;
		        }
		        i++;
		    }
		    //if an end was not found
		    if (i > IMAGE_BUFFER_SIZE || !end)
		    {
		        line_not_found = 1;
		    }
		}else if (end-begin>500){
			line_not_found = 1;
		}
		else//if no begin was found
		{
		    line_not_found = 1;
		}

		//if a line too small has been detected, continues the search
		if(!line_not_found && (end-begin) < MIN_LINE_WIDTH){
			i = end;
			begin = 0;
			end = 0;
			stop = 0;
			wrong_line = 1;
		}
	}while(wrong_line);

	if(line_not_found){
		begin = 0;
		end = 0;
		line_width =  last_width;
	}else{
		line_position = (begin + end)/2; //gives the line position.
		line_width = last_width = (end - begin);

	}


}



uint16_t get_line_position(void){
	return line_position;
}

void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO+1, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO+1, CaptureImage, NULL);
}

_Bool get_red_stop(void){
	if (red_stop) {
		red_stop =0;
		return !red_stop;
	}
	return 0;
}

void reset_red_stop(void){
	red_stop=0;
}

void set_red_stop(void){
	red_stop =1;
}

void check_red_stop(uint8_t *red_image,uint8_t *blue_image){

	uint16_t mean_red = 0;
	uint32_t mean_line_red = 0;
	uint32_t mean_line_blue = 0;
	//updates the values of the first and the last pixels of the black line
	 extract_line(blue_image);
	uint16_t begin_line = begin;
	uint16_t end_line = end;

	//computes the mean of the overall red intensity
		for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){

		mean_red += red_image[i];
	}
	mean_red /= IMAGE_BUFFER_SIZE;

	//computes the mean of the red and the blue pixels only
	//where the line is supposed to be
	for(uint16_t i = begin_line; i < end_line ; ++i){

		mean_line_red += red_image[i];
		mean_line_blue += blue_image[i];
	}
	mean_line_red /= (end_line-begin_line);
	mean_line_blue /= (end_line-begin_line);

	//enters the if only when a line is found, the red intensity mean of the line is higher
	//than the overall mean, and the blue line mean is low
	if( (mean_line_red > mean_red*COEFF_MEAN_RED) && (mean_line_blue < BLUE_LINE_AVERAGE) ){
		
		red_stop = 1;
	}
}

void check_black_h_line(uint8_t *image_red,uint8_t *image_blue){

	uint16_t mean_red  = 0;
	uint16_t mean_blue  = 0;
	//computes the mean of the overall red intensity
	for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){

			mean_red += image_red[i];
	}
	mean_red /= IMAGE_BUFFER_SIZE;

	//computes the mean of the overall blue intensity
		for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){

				mean_blue += image_blue[i];
		}
		mean_blue /= IMAGE_BUFFER_SIZE;


	//enters the if when a black horizontal line is spotted
	//meaning that the robot found the continuity of the path
	if (mean_red < BLACK_H_LINE_AVERAGE && mean_blue < BLACK_H_LINE_AVERAGE){

		horizontal_line = 1;
	}
}

_Bool get_horizontal_line(void){
	if (horizontal_line) {
		horizontal_line = 0;
		return !horizontal_line;
	}
	return 0;
}

void reset_horizontal_line(void){
	horizontal_line = 0;
}

void set_horizontal_line(void){
	horizontal_line = 1;
}

