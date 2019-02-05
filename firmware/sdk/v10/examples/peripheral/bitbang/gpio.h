#include "nrf_drv_gpiote.h"
#define LEDS_MASK      (0xFFFFFFFF)
#define LEDS_INV_MASK  LEDS_MASK

#define LEDS_OFF(leds_mask) do {  NRF_GPIO->OUTSET = (leds_mask) & (LEDS_MASK & LEDS_INV_MASK); \
                            NRF_GPIO->OUTCLR = (leds_mask) & (LEDS_MASK & ~LEDS_INV_MASK); } while (0)

#define LEDS_ON(leds_mask) do {  NRF_GPIO->OUTCLR = (leds_mask) & (LEDS_MASK & LEDS_INV_MASK); \
                           NRF_GPIO->OUTSET = (leds_mask) & (LEDS_MASK & ~LEDS_INV_MASK); } while (0)

#define LED_IS_ON(leds_mask) ((leds_mask) & (NRF_GPIO->OUT ^ LEDS_INV_MASK) )

#define LEDS_INVERT(leds_mask) do { uint32_t gpio_state = NRF_GPIO->OUT;      \
                              NRF_GPIO->OUTSET = ((leds_mask) & ~gpio_state); \
                              NRF_GPIO->OUTCLR = ((leds_mask) & gpio_state); } while (0)

#define LEDS_CONFIGURE(leds_mask) do { uint32_t pin;                  \
                                  for (pin = 0; pin < 32; pin++) \
                                      if ( (leds_mask) & (1 << pin) )   \
                                          nrf_gpio_cfg_output(pin); } while (0)

#if 1
#define GPIO_ON(X) 	NRF_GPIO->OUTSET = ( 1UL << X );
#define GPIO_OFF(X) NRF_GPIO->OUTCLR = ( 1UL << X );
#else
#define GPIO_ON(X)  printf("")
#define GPIO_OFF(X) printf("")
#endif
