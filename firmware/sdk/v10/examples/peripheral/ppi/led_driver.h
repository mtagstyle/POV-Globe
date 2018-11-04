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
    uint8_t current_led_byte[NUM_STRIPS];
    uint8_t current_led_index;
    bool    is_active;
};

static led_rgb_t led_strips[NUM_STRIPS][LEDS_PER_STRIP];

// Set all values to 0
initialize_driver()
{
    // Initialize led_driver_info_t
    
    // Initialize strips
}

run_led_state()
{
    // For each strip
        // Mask the MSB of the byte

        // Left shit logic

        // If we are the last bit, shift to the next byte, read it in to the current byte

        // If we are the last byte, then end, no longer active bois
}