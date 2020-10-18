#include <unistd.h>
#include <cstdio>
#include <thread>

#include "Common.h"
#include "ImageProcessor.h"
#include "LedStripPrinter.h"
#include "LedStripDataStore.h"

/*
#include "led_driver.h"
#define NUM_WRAPPERS 4

static led_wrapper_t led_wrappers[NUM_WRAPPERS] = {};

void setup()
{
    for(int i = 0; i < NUM_WRAPPERS; i++)
    {
        led_wrapper_t* led_wrapper_ptr = &(led_wrappers[i]);
        led_strip_init(led_wrapper_ptr, static_cast<led_strip_t>(i+1), WRAPPER_NUM_LEDS);
    }
}

void update_leds()
{
    uint32_t led_arr[WRAPPER_NUM_LEDS] = {0};
    for(int i = 0; i < WRAPPER_NUM_LEDS; i++)
    {
        led_arr[i] = 0x00FF0000;
    }

    for(int i = 0; i < NUM_WRAPPERS; i++)
    {
        led_wrapper_t* led_wrapper_ptr = &(led_wrappers[i]);
        led_strip_write(led_wrapper_ptr, led_arr, WRAPPER_NUM_LEDS);
    }
}

void teardown()
{
    for(int i = 0; i < NUM_WRAPPERS; i++)
    {
        led_wrapper_t* led_wrapper_ptr = &(led_wrappers[i]);
        led_strip_finish(led_wrapper_ptr);
    }
}

int main(int argc, char* argv[])
{
    setup();
    printf("Successfully initialized\n.");
    while(true)
    {
        printf("Writing to LED strips\n.");
        update_leds();
        sleep(1);
    }

    teardown();
    return 0;
}
*/

void setupImage(std::shared_ptr<ImageProcessor> image_processor, uint16_t rows, uint16_t cols)
{
    int offset = 13;
    led_rgb_t color = {0xFF, 0xFF, 0xFF, 0x00};
    int remainder = offset;
    for(int x = 0; x < cols/2; x++)
    {
        for(int y = 0; y < rows/2; y++)
        {
            // Top left
            image_processor->updateLed(x, y, color);
            // Top right
            image_processor->updateLed(x + cols/2, y, color);
            // Bottom left
            image_processor->updateLed(x, y + rows/2, color);
            // Bottom right
            image_processor->updateLed(x + cols/2, y + rows/2, color);
        }

        if(color.red != 0)
        {
            if(color.red >=offset)
            {
                color.red -= (offset-remainder);
                remainder = 0;
            }
            else
            {
                remainder = color.red;
                color.red = 0;
            }
        }
        else if(color.green != 0)
        {
            if(color.green >=offset)
            {
                color.green -= (offset-remainder);
                remainder = 0;
            }
            else
            {
                remainder = color.green;
                color.green = 0;
            }
        }
        else if(color.blue != 0)
        {
            if(color.blue >=offset)
            {
                color.blue -= (offset-remainder);
                remainder = 0;
            }
            else
            {
                remainder = color.blue;
                color.blue = 0;
            }
        }

        printf("color.r=%d, color.g=%d, color.b=%d\n", color.red, color.green, color.blue);
    }
}

int main(int argc, char* argv[])
{
    uint16_t num_strips = 3;
    uint16_t leds_per_strip = 32;
    uint16_t img_rows = 64;
    uint16_t img_cols = 128;

    std::shared_ptr<LedStripDataStore>            datastore       = std::make_shared<LedStripDataStore>(img_cols*2, leds_per_strip);
    std::shared_ptr<ImageProcessor>               image_processor = std::make_shared<ImageProcessor>(datastore, img_rows, img_cols);
    std::shared_ptr<std::vector<LedStripPrinter>> strip_printers  = std::make_shared<std::vector<LedStripPrinter>>();

    uint16_t offset = 0;
    for(int i = 0; i < num_strips; i++)
    {
        strip_printers->emplace_back( datastore, 
                                      static_cast<led_strip_t>(i+1), 
                                      offset,
                                      leds_per_strip,
                                      img_cols );

        offset += img_rows;
    }

    setupImage(image_processor, img_rows, img_cols);

    uint16_t current_pattern = 0;
    printf("Writing to strip\n");
    std::thread t[num_strips];
    while(true)
    {
        
        for(int i = 0; i < num_strips; i++)
        {
            strip_printers->at(i).setPattern(current_pattern);
            strip_printers->at(i).renderStrip();
        }
        
        /*
        strip_printers->at(0).setPattern(current_pattern);
        strip_printers->at(0).renderStrip();
        */
        /*
        for(int i = 0; i < 1; i++)
        {
            t[i].join();
        }
        */
        current_pattern = ((current_pattern + 1) % img_cols);
        usleep(1920);
    }

    return 0;
}