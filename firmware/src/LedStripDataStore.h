#pragma once

#include "Common.h"

#include <vector>

class LedStripDataStore
{
public:
    LedStripDataStore(uint16_t num_strip_positions, uint16_t num_leds_per_strip);
    const led_rgb_t* getStrip(uint16_t strip_idx);
    bool updateStrip(const led_rgb_t* src, uint16_t strip_idx);
    bool updateLed(uint16_t strip_idx, uint16_t led_idx, const led_rgb_t& src);
    uint16_t getNumStrips();
    uint16_t getNumLeds();

private:
    uint16_t mNumStrips;
    uint16_t mNumLeds;
    std::vector<std::vector<led_rgb_t>> mData;
};