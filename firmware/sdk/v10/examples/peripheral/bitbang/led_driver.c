#include "led_driver.h"
#include "gpio.h"

static led_rgb_t         led_strips[NUM_STRIPS][LEDS_PER_STRIP];
static uint8_t           led_pins[NUM_STRIPS];
static led_driver_info_t driver_info;

// Set all values to 0
initialize_driver()
{
    // Initialize led_driver_info_t
	memset(&driver_info, 0, sizeof(driver_info));
    // Initialize strips
	memset(&led_strips, 0, sizeof(led_strips));
}

/*
Promises: Prints out to all connected LED strips the color data set in "led_strips"
Requires: The "led_strips" data struct is properly loaded with the correct values
*/
void run_led_state()
{
	// Iterate through every single byte in the LED strip length R,G,B,R,G,B
    for(uint8_t i = 0; i < LEDS_PER_STRIP * sizeof(led_rgb_t); i++)
    {
		// For each strip
		for (uint8_t j = 0; j < NUM_STRIPS; j++)
		{
			// Read in NUM_STRIPS bytes that will be attempting to print
			driver_info.current_byte[j] = &(led_strips[j][0])+(sizeof(uint8_t)*i);
		}
		
		// Begin iterating through all bits in the byte
		for (uint8_t j = 0; j < 8; j++)
		{
			// First cycle: Begin turning all GPIOs ON
			for (uint8_t k = 0; k < NUM_STRIPS; k++)
			{
				GPIO_ON(led_pins[k]);
			}
			// Second cycle: Check for any 0's, if they are 0, turn them OFF
			for (uint8_t k = 0; k < NUM_STRIPS; k++)
			{
				if ((driver_info.current_byte[k] & 0x80) == 0)
					GPIO_OFF(led_pins[k]);
			}
			// Third cycle: Check for any 1's, if they are 1, turn them OFF
			for (uint8_t k = 0; k < NUM_STRIPS; k++)
			{
				if ((driver_info.current_byte[k] & 0x80) == 0)
					GPIO_OFF(led_pins[k]);
			}
			// Fourth cycle: Shift all bytes down
			for (uint8_t k = 0; k < NUM_STRIPS; k++)
			{
				driver_info.current_byte[k] = driver_info.current_byte[k] << 1;
			}
		}
    }
}
