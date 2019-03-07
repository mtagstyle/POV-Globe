#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__

#include <stdbool.h>

#define LED_0_BITMASK 0b0001
#define LED_1_BITMASK 0b0010
#define LED_2_BITMASK 0b0100
#define LED_3_BITMASK 0b1000

#define NUM_STRIPS     4
#define LEDS_PER_STRIP 32

#define BITSTREAM_LEN  384

#define SLEEP_CYCLE1() __asm__("nop\n\tnop\n\t")							// 5 cycles at 8 MHz
#define SLEEP_CYCLE2() __asm__("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\t")		// 10 cycles at 8 MHz
#define SLEEP_CYCLE3() __asm__("nop\n\t")									// 5 cycles at 8 MHz

typedef uint8_t gpio_bitstream_t[BITSTREAM_LEN];

bool led_driver_initialize();
bool led_driver_print_bitstream(gpio_bitstream_t *bit_ptr);
void test_fs_config();
#endif