#include <string.h>
#include "stdint.h"
#include "led_driver.h"
#include "gpio.h"
#include "nrf_delay.h"

static combined_bitstream_t stream = {0};
static led_rgb_t         	led_strips[NUM_STRIPS][LEDS_PER_STRIP];
static uint8_t          	led_pins[NUM_STRIPS];
static led_driver_info_t 	driver_info;

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

void image_data_to_driver_data(combined_bitstream_t* driver_data, led_rgb_t* image_data[NUM_STRIPS][LEDS_PER_STRIP])
{
	if(image_data != NULL && driver_data != NULL)
	{
		// For each byte in the LED strip (Each LED is 3 bytes long (RGB))
		for(uint8_t i = 0; i < LEDS_PER_STRIP*sizeof(led_rgb_t); i++)
		{
			uint8_t current_unmasked_bits0 = (*((uint8_t*)&(image_data[0][0]) + (i)));
			uint8_t current_unmasked_bits1 = (*((uint8_t*)&(image_data[1][0]) + (i)));
			uint8_t current_unmasked_bits2 = (*((uint8_t*)&(image_data[2][0]) + (i)));
			uint8_t current_unmasked_bits3 = (*((uint8_t*)&(image_data[3][0]) + (i)));

			// For each bit in the byte
			uint8_t num_shift = 7;
			for(uint8_t j = 0x80; j >= 0x01; j = j >> 1)
			{
				// Storage for masking the bits
				uint8_t current_masked_bits0 = (current_unmasked_bits0 & j) >> (num_shift);		// Bit position 0
				uint8_t current_masked_bits1 = (current_unmasked_bits1 & j) >> (num_shift - 1); // Bit position 1
				uint8_t current_masked_bits2 = (current_unmasked_bits2 & j) >> (num_shift - 2); // Bit position 2
				uint8_t current_masked_bits3 = (current_unmasked_bits3 & j) >> (num_shift - 3); // Bit position 3

				uint8_t result = current_masked_bits0 | current_masked_bits1 | current_masked_bits2 | current_masked_bits3;
				driver_data->bitstream[(i*8)+(7-num_shift)] = result;

				num_shift--;
			}
		}
	}
}

/*
Promises: Prints out to all connected LED strips the color data set in "led_strips"
Requires: The "led_strips" data struct is properly loaded with the correct values
*/
void run_led_state_optimized()
{
	image_data_to_driver_data(&stream, &led_strips);
	
	// Iterate through every single byte in the LED strip length R,G,B,R,G,B
	register uint32_t INIT_MASK = 0x00;
	// Initialize
	for(uint16_t i = 0; i < NUM_STRIPS; i++)
	{
		INIT_MASK |= (1 << led_pins[i]);
	}

	for (uint16_t i = 0; i < 8*LEDS_PER_STRIP*sizeof(led_rgb_t); i++)
	{
		// For each strip, reaad in the byte to print
		register uint32_t TIME0_MASK = 0;
		register uint8_t lut = stream.bitstream[i];

		switch(lut)
		{
			case 0b0000:
				TIME0_MASK = 1;
			break;
			case 0b0001:
				TIME0_MASK = 2;
			break;
			case 0b0010:
				TIME0_MASK = 3;
			break;
			case 0b0011:
				TIME0_MASK = 4;
			break;
			case 0b0100:
				TIME0_MASK = 5;
			break;
			case 0b0101:
				TIME0_MASK = 6;
			break;
			case 0b0110:
				TIME0_MASK = 7;
			break;
			case 0b0111:
				TIME0_MASK = 8;
			break;
			case 0b1000:
				TIME0_MASK = 9;
			break;
			case 0b1001:
				TIME0_MASK = 10;
			break;
			case 0b1010:
				TIME0_MASK = 11;
			break;
			case 0b1011:
				TIME0_MASK = 12;
			break;
			case 0b1100:
				TIME0_MASK = 13;
			break;
			case 0b1101:
				TIME0_MASK = 14;
			break;
			case 0b1110:
				TIME0_MASK = 15;
			break;
			case 0b1111:
				TIME0_MASK = 16;
			break;
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
