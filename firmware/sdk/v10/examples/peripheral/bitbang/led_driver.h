#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__

#include <stdbool.h>

#define LED_0_BITMASK 0b0001
#define LED_1_BITMASK 0b0010
#define LED_2_BITMASK 0b0100
#define LED_3_BITMASK 0b1000

#define NUM_STRIPS     4
#define LEDS_PER_STRIP 32

#define IMG_COLUMNS    128
#define IMG_ROWS       64

#define BITMATRIX_PATTERNS 32
#define BITMATRIX_BITS     768

#define SLEEP_CYCLE1() __asm__("nop\n\tnop\n\t")							// 5 cycles at 8 MHz
#define SLEEP_CYCLE2() __asm__("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\t")		// 10 cycles at 8 MHz
#define SLEEP_CYCLE3() __asm__("nop\n\t")									// 5 cycles at 8 MHz

typedef enum { LED_NOTINIT, LED_INIT, LED_ERROR } led_driver_state_t;

/**
 * Representation of a single LED
 */
typedef struct led_rgb
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} __attribute__((packed)) led_rgb_t;

/**
 * The 64x128 image represented as a list of GPIO registers depending on the columns to print out
 */
typedef uint8_t gpio_bitmatrix_t[BITMATRIX_PATTERNS][BITMATRIX_BITS];

/**
 * Main data structure used by driver to run
 */
typedef struct led_driver_info
{
	gpio_bitmatrix_t*  gpio_matrix_ptr;
	led_driver_state_t state;
} led_driver_info_t;

bool led_driver_initialize(gpio_bitmatrix_t *gpio_matrix);
bool led_driver_print_column(uint8_t column);
void test_fs_config();
#endif