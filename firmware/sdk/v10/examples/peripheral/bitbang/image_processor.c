#include "image_processor.h"

bool LoadQuadrant(gpio_bitmatrix_t *bit_matrix, const image_quadrant_t *img, quad_number_t location)
{
    bool result = true;

    if( bit_matrix != NULL && img != NULL )
    {
        // The direction in which we are traversing the quadrant (1 = UP, -1 = DOWN)
        uint8_t y_direction = 0;
        // The bitmask for the LED strip
        uint8_t bitmask = 0;

        // Determine which quadrant to map to
        switch(location)
        {
            case QUAD_TOP_LEFT:
                y_direction = 1;
                bitmask     = LED_0_BITMASK;
            break;

            case QUAD_BOTTOM_LEFT:
                y_direction = -1;
                bitmask     = LED_1_BITMASK;
            break;

            case QUAD_TOP_RIGHT:
                y_direction = 1;
                bitmask     = LED_2_BITMASK;
            break;

            case QUAD_BOTTOM_RIGHT:
                y_direction = -1;
                bitmask     = LED_3_BITMASK;
            break;

            default:
                result = false;
            break;
        }

        // Iterate through each bit in the bit-matrix, and map it to the quadrant
        if(result)
        {
            // For each bit-matrix column (Which represents 2 cols)
            for(uint32_t i = 0; i < BITMATRIX_PATTERNS/4; i++)
            {
                // For each bit matrix byte, there are 2 colummns
                for(uint8_t j = 0; j < 2; j++)
                {
                    // For each bit to written to the pattern
                    for(uint32_t j = 0; j < BITMATRIX_BITS; j++)
                    {
                        // If we're on the first column
                        if(j == 0)
                        {

                        }
                        // Otherwise, we're on the second column
                        else if( j == 1)
                        {

                        }
                    }
                }
            }
        }
    }
    else
    {
        result = false;
    }
    
    return result;
}