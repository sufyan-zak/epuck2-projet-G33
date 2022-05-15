#ifndef PATH_REGULATOR_H
#define PATH_REGULATOR_H

#include <crossroad.h>
/**
 * @brief :		start the Path regulator thread
 *
 * @return :	None
 */
void path_regulator_start(void);

/**
 * @brief :		starts the thread that toggles the LEDs
 *
 * @return :	None
 */
void leds_toggle_start(void);

/**
 * @brief :		turns off all the LEDs
 *
 * @return :	None
 */
void reset_leds(void);

/**
 * @brief :		toggles the different LEDs in function of the state
 *
 * @return :	None
 */
void led_animation(enum state current_state, enum direction current_direction,
													_Bool *sequence, uint8_t *color);
#endif /* PATH_REGULATOR_H */
