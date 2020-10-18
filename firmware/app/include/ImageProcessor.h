#pragma once

#include <memory>

#include "LedStripDataStore.h"

class ImageProcessor
{
public:
    ImageProcessor(std::shared_ptr<LedStripDataStore> data, 
                   uint16_t rows, 
                   uint16_t cols);

    void updateLed(int x, int y, const led_rgb_t& src);
    led_rgb_t& getLed(int x, int y);

    uint16_t getStripIndex(uint16_t x, uint16_t y);
    uint16_t getLedIndex(uint16_t x, uint16_t y);

private:
    std::shared_ptr<LedStripDataStore> mDataStore;
    uint16_t mImgRows;
    uint16_t mImgCols;
};