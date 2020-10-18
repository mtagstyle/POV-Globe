#pragma once

#include <stdint.h>

typedef struct __attribute__((__packed__)) led_rgb
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t pad;
} led_rgb_t;