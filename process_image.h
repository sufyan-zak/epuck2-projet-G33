#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

float get_distance_cm(void);
uint16_t get_line_position(void);
void process_image_start(void);
_Bool get_red_stop(void);
void check_red_stop(uint8_t *red_image,uint8_t *blue_image);
void reset_red_stop(void);
void set_red_stop(void);
#endif /* PROCESS_IMAGE_H */