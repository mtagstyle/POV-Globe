#define NUM_STRIPS     4
#define LEDS_PER_STRIP 32

// Rememba 2 pack dis struct
typedef struct led_rgb
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} __attribute__((packed)) led_rgb_t;

typedef struct led_driver_info_t
{
    uint8_t   current_byte[NUM_STRIPS];
    bool      is_active;
};

void initialize_driver()
void run_led_state()