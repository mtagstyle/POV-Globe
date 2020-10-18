#include "ImageProcessor.h"

#include <stdexcept>

ImageProcessor::ImageProcessor(std::shared_ptr<LedStripDataStore> data, uint16_t rows, uint16_t cols)
    :mDataStore(data),
     mImgRows(rows),
     mImgCols(cols)
{
}
                   
void ImageProcessor::updateLed(int x, int y, const led_rgb_t& src)
{
    uint16_t strip_index = getStripIndex(x, y);
    uint16_t led_index = getLedIndex(x, y);
    
    mDataStore->updateLed(strip_index, led_index, src);
}

led_rgb_t& ImageProcessor::getLed(int x, int y)
{
    throw std::domain_error("Not Implemented.\n");
}

uint16_t ImageProcessor::getStripIndex(uint16_t x, uint16_t y)
{
    uint16_t offset = -1;
    uint16_t strip_index = -1;

    // Top half of screen
    if(y >= 0 && y < (mImgRows/2))
    {
        offset = 0;
    }
    // Bottom half of screen
    else if(y >= (mImgRows/2) && y < mImgRows)
    {
        offset = mImgCols;
    }
    else
    {
        throw std::out_of_range("Y coordinate is not in range");
    }

    if(x >= 0 && x < mImgCols)
    {
        strip_index = x + offset;
    }
    else
    {
        throw std::out_of_range("X coordinate is not in range");
    }

    return strip_index;
}

uint16_t ImageProcessor::getLedIndex(uint16_t x, uint16_t y)
{
    uint16_t led_index = -1;

    // Top half of screen - Leds are counted bottom up
    if(y >= 0 && y < (mImgRows/2))
    {
        led_index = (mImgRows/2 - 1) - y;
    }
    // Bottom half of screen - Leds are counted top down
    else if(y >= (mImgRows/2) && y < mImgRows)
    {
        led_index = y - mImgRows/2;
    }
    else
    {
        throw std::out_of_range("Y coordinate is not in range");
    }

    return led_index;
}
