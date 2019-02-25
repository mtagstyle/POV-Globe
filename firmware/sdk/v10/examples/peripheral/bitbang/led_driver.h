#define NUM_STRIPS     4
#define LEDS_PER_STRIP 32

#define SLEEP_CYCLE1() __asm__("nop\n\tnop\n\t")							// 5 cycles at 8 MHz
#define SLEEP_CYCLE2() __asm__("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\t")		// 10 cycles at 8 MHz
#define SLEEP_CYCLE3() __asm__("nop\n\t")									// 5 cycles at 8 MHz

// Rememba 2 pack dis struct
typedef struct led_rgb
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} __attribute__((packed)) led_rgb_t;

typedef struct led_driver_info
{
	uint8_t   current_byte[NUM_STRIPS];
	uint8_t   is_active;
} led_driver_info_t;

typedef struct combined_bitstream
{
	uint8_t bitstream[8*LEDS_PER_STRIP*sizeof(led_rgb_t)];
} combined_bitstream_t;

void initialize_driver();
void test_initialize_driver();
//void run_led_state();
void run_led_state_optimized();
