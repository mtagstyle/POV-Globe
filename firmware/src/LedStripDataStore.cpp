#include "LedStripDataStore.h"

#include <string.h>

LedStripDataStore::LedStripDataStore(uint16_t num_strip_positions, uint16_t num_leds_per_strip)
    : mNumStrips(num_strip_positions),
      mNumLeds(num_leds_per_strip)
{
    // Preallocate the datastore with the specified values
    mData.resize(num_strip_positions, std::vector<led_rgb_t>());
    for(auto& it : mData)
    {
        it.resize(num_leds_per_strip, led_rgb_t());
    }
}

const led_rgb_t* LedStripDataStore::getStrip(uint16_t strip_idx)
{
    led_rgb_t* ret = nullptr;

    if(strip_idx < mData.size())
    {
        ret = mData.at(strip_idx).data();
    }

    return ret;
}

bool LedStripDataStore::updateStrip(const led_rgb_t* src, uint16_t strip_idx)
{
    bool result = false;

    if(strip_idx < mNumStrips)
    {
        memcpy(mData.at(strip_idx).data(), src, mNumLeds*sizeof(led_rgb_t));
        result = true;
    }

    return result;
}

bool LedStripDataStore::updateLed(uint16_t strip_idx, uint16_t led_idx, const led_rgb_t& src)
{
    bool result = false;

    if(strip_idx < mNumStrips && led_idx < mNumLeds)
    {
        mData.at(strip_idx).at(led_idx) = src;
        result = true;
    }

    return result;
}

uint16_t LedStripDataStore::getNumStrips()
{
    return mNumStrips;
}

uint16_t LedStripDataStore::getNumLeds()
{
    return mNumLeds;
}