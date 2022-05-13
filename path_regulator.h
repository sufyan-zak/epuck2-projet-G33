#ifndef PATH_REGULATOR_H
#define PATH_REGULATOR_H

/**
 * @brief :		start the Path regulator thread
 *
 * @return :	None
 */
void path_regulator_start(void);

/**
 * @brief :		starts the thread responsible of leds
 *
 * @return :	None
 */
void leds_toggle_start(void);

/**
 * @brief :		turns off all the leds
 *
 * @return :	None
 */
void reset_leds(void);

#endif /* PATH_REGULATOR_H */
