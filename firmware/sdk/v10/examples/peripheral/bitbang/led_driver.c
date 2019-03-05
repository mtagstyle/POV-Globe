#include <string.h>
#include "stdint.h"
#include "led_driver.h"
#include "gpio.h"
#include "nrf_delay.h"
#include "pstorage.h"
#include "nrf_soc.h"

#define PSTORAGE_MAX_APPLICATIONS 2

#define NRF_EVT_FLASH_OPERATION_SUCCESS 2
#define NRF_EVT_FLASH_OPERATION_ERROR 3

static led_driver_info_t driver_info;

// Event Notification Handler.
static void example_cb_handler(pstorage_handle_t  * handle,
                               uint8_t              op_code,
                               uint32_t             result,
                               uint8_t            * p_data,
                               uint32_t             data_len)
{
}

static pstorage_handle_t handle;

void test_fs_config()
{

	// Initialize
	uint32_t retval = 0;
	retval = pstorage_init();
	if(retval == NRF_SUCCESS)
	{
		// Module initialization successful.
	}
	else
	{
		// Initialization failed, take corrective action.
	}
	pstorage_module_param_t param;
		
	param.block_size  = PSTORAGE_MAX_BLOCK_SIZE;
	param.block_count = 1;
	param.cb          = example_cb_handler;
		
	retval = pstorage_register(&param, &handle);
	if (retval == NRF_SUCCESS)
	{
		// Registration successful.
	}
	else
	{
		// Failed to register, take corrective action.
	}
}

void load_shit(uint8_t *dst)
{
	// Load
	uint32_t retval = 0;
	pstorage_handle_t block_handle;

	// Registration successfully completed, base_handle is identifier for allocated blocks.
	// Request to get identifier for 3rd block. 
	retval = pstorage_block_identifier_get(&handle, 0, &block_handle);
	retval = pstorage_load(dst, &block_handle, 768, 0);
}

bool led_driver_initialize(gpio_bitmatrix_t *gpio_matrix)
{
	bool result = true;

	memset(&driver_info, 0, sizeof(driver_info));

	if(gpio_matrix != NULL)
	{
		// GPIOs
		nrf_gpio_cfg_output( 12 ); 
		nrf_gpio_cfg_output( 15 ); 
		nrf_gpio_cfg_output( 5 ); 
		nrf_gpio_cfg_output( 23 ); 

		// Slight delay to let GPIO configuration finish 
		nrf_delay_ms( 100 );

		// Driver Information
		driver_info.gpio_matrix_ptr = gpio_matrix;
		driver_info.state 			= LED_INIT;

		//NRF_GPIO->OUTSET = 4096;
	}
	else
	{
		result = false;
	}
	
	return result;
}

/*
	Promises: Runs the main algorithm to print to all running LED strips at a time

        ----------------------------
	    | Byte 0 | Byte 1 | Byte 2 |
		-----------------------------
	    |  Cols  |  Cols  |  Cols  |
	    |  0 - 1 |  2 - 3 |  4 - 5 |
	-----------------------------------------------
	0   |11110000|00001111|
	1   |
	2   |
	3   |
	4   |
	5   |
	6   |
	7   |
	.   |
	.   |
	.   |
	767 |
	
	The data structure used is a 2D array, where:
	   - Each column represents a column of the image to display
	   - Each row represents a BIT to write to the WS2812B
*/
bool led_driver_print_column(uint8_t column)
{
	// Iterate through every single byte in the LED strip length R,G,B,R,G,B
	bool	 result    		 = true;
	static uint8_t num_shift = NUM_STRIPS;
	uint32_t INIT_MASK 		 = 4096;

	uint8_t temp[768] = {0};

	NRF_GPIO->OUTSET = INIT_MASK;
	load_shit(temp);
	NRF_GPIO->OUTCLR = INIT_MASK;

	if( (column < IMG_COLUMNS/2) && (driver_info.state == LED_INIT) )
	{
		for (uint16_t i = 0; i < BITMATRIX_BITS; i++)
		{
			// For each strip, read in the byte to print, each byte represents 2 columns
			uint8_t data = temp[i];//driver_info.gpio_matrix_ptr[column][i];

			// Take either the upper or lower half of the byte
			uint32_t TIME0_MASK = data >> num_shift;
		
			// Turn them all on
			NRF_GPIO->OUTSET = INIT_MASK;
			// TIME0, turn the appropriate ones off
			SLEEP_CYCLE1();
			NRF_GPIO->OUTCLR = TIME0_MASK;
			SLEEP_CYCLE2();
			NRF_GPIO->OUTCLR = INIT_MASK;
		}

		// Instead of using a branch or modulus to determine whether to take the upper lower half
		// of the byte, we just do this to alternate between 4 bits and 0 bits to shift
		num_shift = NUM_STRIPS - num_shift;
	}
	else
	{
		result = false;
	}
	
	return result;
}

/*
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
				uint8_t gpio_array = 0; 



				driver_data->bitstream[0][(i*8)+(7-num_shift)] = gpio_array;

				num_shift--;
			}
		}
	}
}
*/
