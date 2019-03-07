#include <string.h>
#include "stdint.h"
#include "led_driver.h"
#include "gpio.h"
#include "nrf_delay.h"

bool led_driver_initialize()
{
	bool result = true;

	// GPIOs
	nrf_gpio_cfg_output( 12 ); 
	nrf_gpio_cfg_output( 15 ); 
	nrf_gpio_cfg_output( 5 ); 
	nrf_gpio_cfg_output( 23 ); 

	// Slight delay to let GPIO configuration finish 
	nrf_delay_ms( 100 );

	return result;
}

/*
	Promises: Writes the contents given by "bitstream_ptr" to the 4 WS2812B LEDs.

                       -------------------------------------------------------------
                       |                           Byte0                           |
                       |-----------------------------------------------------------|	                  
					   | LED0   LED1   LED2   LED3   | LED0   LED1   LED2    LED3  |
					   |-----------------------------------------------------------|
	gpio_bitstream_t   |   0       1     0      1    |   0       1     0       1   |
                       |-----------------------------------------------------------|
					   |      LED Strips Bit 0       |       LED Strips Bit 1      |
					   -------------------------------------------------------------

	Requires: A pointer to a bitstream is provided, GPIOs are all initialized	                  
*/
bool led_driver_print_bitstream(gpio_bitstream_t *bitstream_ptr)
{
	// Iterate through every single byte in the LED strip length R,G,B,R,G,B
	bool result    		 	 = true;
	static uint8_t num_shift = NUM_STRIPS;
	uint32_t INIT_MASK 		 = 4096;

	if(bitstream_ptr != NULL)
	{
		for (uint16_t i = 0; i < BITSTREAM_LEN*2; i++)
		{
			// For each strip, read in the byte to print, each byte represents 2 columns
			uint8_t data = bitstream_ptr[i];

			// Take either the upper or lower half of the byte
			uint32_t TIME0_MASK = data >> num_shift;
		
			// Turn them all on
			NRF_GPIO->OUTSET = INIT_MASK;
			// TIME0, turn the appropriate ones off
			SLEEP_CYCLE1();
			NRF_GPIO->OUTCLR = TIME0_MASK;
			//SLEEP_CYCLE2();
			// Instead of using a branch or modulus to determine whether to take the upper lower half
			// of the byte, we just do this to alternate between 4 bits and 0 bits to shift
			num_shift = NUM_STRIPS - num_shift;
			NRF_GPIO->OUTCLR = INIT_MASK;
		}
	}
	else
	{
		result = false;
	}
	
	return result;
}
