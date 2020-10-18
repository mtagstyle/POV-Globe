#include "LedStripPrinter.h"

LedStripPrinter::LedStripPrinter( std::shared_ptr<LedStripDataStore> data,
                                  led_strip_t strip,
                                  uint16_t    offset, 
                                  uint16_t    num_leds, 
                                  uint16_t    num_patterns)
    : mDataStore(data),
      //mLedWrapper(std::make_unique<led_wrapper_t>()),
      mOffset(offset),
      mNumLeds(num_leds),
      mNumPatterns(num_patterns),
      mCurrentPattern(0)
{
    //led_strip_init(mLedWrapper.get(), strip, num_leds);
}

LedStripPrinter::LedStripPrinter(LedStripPrinter&& other)
    : mDataStore(std::move(other.mDataStore)),
      //mLedWrapper(std::move(other.mLedWrapper)),
      mOffset(other.mOffset),
      mNumLeds(other.mNumLeds),
      mNumPatterns(other.mNumPatterns),
      mCurrentPattern(other.mCurrentPattern)
{
}

LedStripPrinter::~LedStripPrinter()
{
/*
    if(mLedWrapper)
    {
        printf("tearing down led resource\n");
        led_strip_finish(mLedWrapper.get());
    }
*/
}

bool LedStripPrinter::setPattern(uint16_t pattern_num)
{
    bool result = false;
    if(pattern_num < mNumPatterns)
    {
        mCurrentPattern = pattern_num;
        result = true;
    }

    return result;
}

void LedStripPrinter::renderStrip()
{
    // The pattern to print is defined by a rolling window applied over the datastore
    uint16_t total_strips =  mDataStore->getNumStrips();
    uint16_t led_strip_index = (mCurrentPattern + mOffset) % total_strips;
    const led_rgb_t* led_ptr = mDataStore->getStrip(led_strip_index);
    //led_strip_write(mLedWrapper.get(), (uint32_t*) led_ptr, mNumLeds);
}
