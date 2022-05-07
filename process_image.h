#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H


uint16_t get_line_position(void);
void process_image_start(void);


/**
 * @brief			the following function sets the value of red_stop indicating
 * 					that a red line has been found just after a new image is processed
 *
 * @return			none
 */
void check_red_stop(uint8_t *red_image,uint8_t *blue_image);

_Bool get_red_stop(void);
void reset_red_stop(void);
void set_red_stop(void);

int get_horizontal_line(void);

void reset_horizontal_line(void);
void check_black_h_line(uint8_t *red_image);
void set_horizontal_line(void);
void set_check_test(void);
void reset_check_test(void);
#endif /* PROCESS_IMAGE_H */
