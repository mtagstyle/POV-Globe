#include <memory>

#include "led_driver.h"
#include "LedStripDataStore.h"

class LedStripPrinter
{
public:
    LedStripPrinter(std::shared_ptr<LedStripDataStore> data,
                    led_strip_t strip,
                    uint16_t    offset, 
                    uint16_t    num_leds, 
                    uint16_t    num_patterns);
    LedStripPrinter(LedStripPrinter&& other);
    ~LedStripPrinter();
    bool setPattern(uint16_t pattern_num);
    void renderStrip();

private:
    std::shared_ptr<LedStripDataStore> mDataStore;
    std::unique_ptr<led_wrapper_t>     mLedWrapper;
    uint16_t                           mOffset;
    uint16_t                           mNumLeds;
    uint16_t                           mNumPatterns;
    uint16_t                           mCurrentPattern;
};