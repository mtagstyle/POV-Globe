#include <string.h>
#include "stdint.h"
#include "led_driver.h"
#include "gpio.h"
#include "nrf_delay.h"

static led_rgb_t         led_strips[NUM_STRIPS][LEDS_PER_STRIP];
static uint8_t           led_pins[NUM_STRIPS];
static led_driver_info_t driver_info;

void rgb_led_driver_init()
{
    nrf_gpio_cfg_output( 12 ); 
	nrf_gpio_cfg_output( 15 ); 
    nrf_gpio_cfg_output( 5 ); 
    nrf_gpio_cfg_output( 23 ); 

    // Slight delay to let GPIO configuration finish 
    nrf_delay_ms( 100 );
}

// Set all values to 0
void initialize_driver()
{
	// Initialize GPIOs
	rgb_led_driver_init();
	// Initialize led_driver_info_t
	memset(&driver_info, 0, sizeof(driver_info));
	// Initialize pins
	memset(&led_pins, 0, sizeof(led_pins));
	// Initialize strips
	memset(&led_strips, 0, sizeof(led_strips));
}

void test_initialize_driver()
{
	led_pins[0] = 12;
	led_pins[1] = 15;
	led_pins[2] = 5;
	led_pins[3] = 23;
	uint8_t* led_strip_ptr = (uint8_t*)led_strips;
	memset(led_strip_ptr, 0xAA, sizeof(led_rgb_t)*NUM_STRIPS*LEDS_PER_STRIP);
}

/*
Promises: Prints out to all connected LED strips the color data set in "led_strips"
Requires: The "led_strips" data struct is properly loaded with the correct values
*/
void run_led_state_optimized()
{
	// Iterate through every single byte in the LED strip length R,G,B,R,G,B
	register uint32_t current_byte = 0;
	register uint32_t INIT_MASK = 0x00;
	// Initialize
	for(uint8_t i = 0; i < NUM_STRIPS; i++)
	{
		INIT_MASK |= (1 << led_pins[i]);
	}

	for (uint8_t i = 0; i < LEDS_PER_STRIP * sizeof(led_rgb_t); i++)
	{
		// For each strip
		uint8_t num_shift = 24;
		for (uint8_t j = 0; j < NUM_STRIPS; j++)
		{
			// Read in NUM_STRIPS bytes that will be attempting to print
			//current_byte[j] = *((uint8_t*)&(led_strips[j][0]) + (i));
			current_byte |= (*((uint8_t*)&(led_strips[j][0]) + (i))) << num_shift;
			num_shift -= 8;
		}

		register uint32_t TIME0_MASK = 0;
		register uint16_t current_masked_bits0 = 0;
		register uint16_t current_masked_bits1 = 0;
		register uint16_t current_masked_bits2 = 0;
		register uint16_t current_masked_bits3 = 0;

		// Store every single bit mask (4 of them) in 4 bytes
		for(register uint16_t j = 0x8080; j >= 0x0101; j = j >> 1)
		{
			TIME0_MASK = 0;
			current_masked_bits_top = current_byte & j;
			current_masked_bits0 = (current_masked_bits & 0xFF000000) >> 24;
			current_masked_bits1 = (current_masked_bits & 0x00FF0000) >> 16;
			current_masked_bits2 = (current_masked_bits & 0x0000FF00) >> 8;
			current_masked_bits3 = current_masked_bits & 0x000000FF;

			if(!(current_masked_bits0))
			{
				TIME0_MASK |= (1 << 12);
			}
			if(!(current_masked_bits1))
			{
				TIME0_MASK |= (1 << 15);
			}
			if(!(current_masked_bits2))
			{
				TIME0_MASK |= (1 << 5);
			}
			if(!(current_masked_bits3))
			{
				TIME0_MASK |= (1 << 23);
			}

			// Turn them all on
			NRF_GPIO->OUTSET = INIT_MASK;
			// TIME0, turn the appropriate ones off
			SLEEP_CYCLE1();
			NRF_GPIO->OUTCLR = TIME0_MASK;
			// TIME1, turn the appropriate ones off	
			SLEEP_CYCLE2();
			NRF_GPIO->OUTCLR = INIT_MASK;
			// Begin writing after this next cycle ends
			//SLEEP_CYCLE3();
		}
	}
}