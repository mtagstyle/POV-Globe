#define NUM_STRIPS     4
#define LEDS_PER_STRIP 32


// Rememba 2 pack dis struct
typedef struct led_rgb_t
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

typedef struct led_driver_info_t
{
    led_rgb_t current_led[NUM_STRIPS];
    bool      is_active;
};

static led_rgb_t         led_strips[NUM_STRIPS][LEDS_PER_STRIP];
static uint8_t           led_pins[NUM_STRIPS];
static led_driver_info_t driver_info;

// Set all values to 0
initialize_driver()
{
    // Initialize led_driver_info_t
    
    // Initialize strips
}

run_led_state()
{
    for(uint8_t i = 0; i < LEDS_PER_STRIP; i++)
    {
        //Read in the LED
        for(uint8_t j = 0; j < NUM_STRIPS; j++)
        {
            driver_info.current_led[j] = led_strips[j][i];
        }
        for(uint8_t k = 0; k < 8; k++)
        {
            // Mask the MSB of the byte

            // Left shit logic

            // If we are the last bit, shift to the next byte, read it in to the current byte

            // If we are the last byte, then end, no longer active bois
        }
    }
}