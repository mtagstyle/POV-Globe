/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
*
* @defgroup ppi_example_main main.c
* @{
* @ingroup ppi_example
* @brief PPI Example Application main file.
*
* This file contains the source code for a sample application using PPI to communicate between timers.
*
*/


#define GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS 1
#define GPIOTE_CONFIG_IRQ_PRIORITY APP_IRQ_PRIORITY_LOW

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "app_error.h"
#include "boards.h"
#include "nordic_common.h"

#define TIMER0_COMPARE_VALUE 5  // (0.35 us/(0.0625 us/tick)) == ~ 5.6  ticks)
#define TIMER1_COMPARE_VALUE 15  // (0.9  us/(0.0625 us/tick)) == ~ 14.4 ticks)
#define TIMER2_COMPARE_VALUE 20 // (1.25 us/(0.0625 us/tick)) ==   20 ticks  )

/**
 * Frequency     - 16 MHz
 * Mode          - Timer mode
 * Bit Width     - 32
 * Callback args - None
 */
nrf_drv_timer_config_t       timer_config = {NRF_TIMER_FREQ_16MHz, NRF_TIMER_MODE_TIMER, NRF_TIMER_BIT_WIDTH_32, 1, NULL};
nrf_drv_gpiote_out_config_t  gpio_config  = {NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW, true};

const nrf_drv_timer_t timer0 = NRF_DRV_TIMER_INSTANCE(0);
const nrf_drv_timer_t timer1 = NRF_DRV_TIMER_INSTANCE(1);
const nrf_drv_timer_t timer2 = NRF_DRV_TIMER_INSTANCE(2);

#define GPIO_OUTPUT_PIN_NUMBER 12

nrf_ppi_channel_t ppi_channel0, ppi_channel1, ppi_channel2, ppi_channel3, ppi_channel4, ppi_channel5, ppi_channel6;

static uint8_t toggle = 0;

void start_shit()
{
    // Turn GPIO on
    nrf_drv_gpiote_out_task_force(GPIO_OUTPUT_PIN_NUMBER, 1);

    // Enable timer 1 hook first
    nrf_drv_ppi_channel_enable(ppi_channel1);

    // Reset all timers and turn them on
    nrf_drv_timer_enable(&timer0);
    nrf_drv_timer_enable(&timer1);
    nrf_drv_timer_enable(&timer2);
}

static void init_gpio()
{
    nrf_drv_gpiote_init();
    nrf_drv_gpiote_out_init(GPIO_OUTPUT_PIN_NUMBER, &gpio_config);
    nrf_drv_gpiote_out_task_enable(GPIO_OUTPUT_PIN_NUMBER);
}

static void init_timer()
{
}

/** @brief Function for initializing the PPI peripheral.
*/

static void init_ppi(void)
{
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    init_timer();
    init_gpio();
    init_ppi();

    start_shit();

    // Loop and increment the timer count value and capture value into LEDs. @note counter is only incremented between TASK_START and TASK_STOP.
    while (true)
    {
        
    }
}


/** @} */