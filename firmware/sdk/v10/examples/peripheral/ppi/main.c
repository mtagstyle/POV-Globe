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

#define TIMER0_COMPARE_VALUE 6  // (0.35 us/(0.0625 us/tick)) == ~ 5.6  ticks)
#define TIMER1_COMPARE_VALUE 14 // (0.9  us/(0.0625 us/tick)) == ~ 14.4 ticks)
#define TIMER2_COMPARE_VALUE 20 // (1.25 us/(0.0625 us/tick)) ==   20 ticks  )

/**
 * Frequency     - 16 MHz
 * Mode          - Timer mode
 * Bit Width     - 32
 * Callback args - None
 */
nrf_drv_timer_config_t       timer_config = {NRF_TIMER_FREQ_16MHz, NRF_TIMER_MODE_TIMER, NRF_TIMER_BIT_WIDTH_32, 1, NULL};
nrf_drv_gpiote_out_config_t  gpio_config  = {NRF_GPIOTE_POLARITY_HITOLO, NRF_GPIOTE_INITIAL_VALUE_LOW, true};

const nrf_drv_timer_t timer0 = NRF_DRV_TIMER_INSTANCE(0);
const nrf_drv_timer_t timer1 = NRF_DRV_TIMER_INSTANCE(1);
const nrf_drv_timer_t timer2 = NRF_DRV_TIMER_INSTANCE(2);

#define GPIO_OUTPUT_PIN_NUMBER 12

nrf_ppi_channel_t ppi_channel1, ppi_channel2;

// Timer even handler. Not used since timer is used only for PPI.
void timer_event_handler(nrf_timer_event_t event_type, void * p_context)
{
    nrf_drv_timer_clear(&timer0);
    nrf_drv_timer_clear(&timer1);
    nrf_drv_timer_clear(&timer2);

    // Turn GPIO on
    nrf_drv_gpiote_out_task_force(GPIO_OUTPUT_PIN_NUMBER, 1);

    // Reset all timers and turn them on
    //nrf_drv_timer_resume(&timer0);
    nrf_drv_timer_resume(&timer1);
    nrf_drv_timer_resume(&timer2);
}

void start_shit()
{
    // Turn GPIO on
    nrf_drv_gpiote_out_task_force(GPIO_OUTPUT_PIN_NUMBER, 1);

    // Reset all timers and turn them on
    nrf_drv_timer_enable(&timer0);
    nrf_drv_timer_enable(&timer1);
    nrf_drv_timer_enable(&timer2);
}

static void init_gpio()
{
    nrf_drv_gpiote_init();
    nrf_drv_gpiote_out_init(GPIO_OUTPUT_PIN_NUMBER, &gpio_config);
}

static void init_timer()
{
    uint32_t error = NRF_SUCCESS;

    // Initialize timers
    error = nrf_drv_timer_init(&timer0, &timer_config, timer_event_handler);
    APP_ERROR_CHECK(error);
    error = nrf_drv_timer_init(&timer1, &timer_config, timer_event_handler);
    APP_ERROR_CHECK(error);
    error = nrf_drv_timer_init(&timer2, &timer_config, timer_event_handler);
    APP_ERROR_CHECK(error);

    // Timer 0, cutoff should be 0.35 us
    nrf_drv_timer_extended_compare(&timer0, NRF_TIMER_CC_CHANNEL0, TIMER0_COMPARE_VALUE, NRF_TIMER_SHORT_COMPARE0_STOP_MASK, false);
    APP_ERROR_CHECK(error);

    // Timer 1, cutoff should be 0.9 us
    nrf_drv_timer_extended_compare(&timer1, NRF_TIMER_CC_CHANNEL1, TIMER1_COMPARE_VALUE, NRF_TIMER_SHORT_COMPARE1_STOP_MASK, false);
    APP_ERROR_CHECK(error);

    // Timer 2, cutoff should be 1.25 us, interrupt here since it is the end of the sequence
    nrf_drv_timer_extended_compare(&timer2, NRF_TIMER_CC_CHANNEL2, TIMER2_COMPARE_VALUE, NRF_TIMER_SHORT_COMPARE2_STOP_MASK, true);
    APP_ERROR_CHECK(error);
}

/** @brief Function for initializing the PPI peripheral.
*/

static void init_ppi(void)
{
    uint32_t err_code = NRF_SUCCESS;

    // Initialize PPI peripheral
    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    // Configure 1st available PPI channel to stop TIMER0 counter on TIMER1 COMPARE[0] match, which is every even number of seconds.
    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel1);
    APP_ERROR_CHECK(err_code);
    
    // Connect the timer1 event to toggle the gpio (Should turn off)
    err_code = nrf_drv_ppi_channel_assign(ppi_channel1, nrf_drv_timer_event_address_get(&timer1, NRF_TIMER_EVENT_COMPARE1), nrf_drv_gpiote_out_task_addr_get(GPIO_OUTPUT_PIN_NUMBER));

    APP_ERROR_CHECK(err_code);

    // Enable both configured PPI channel and GPIO Task
    err_code = nrf_drv_ppi_channel_enable(ppi_channel1);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_out_task_enable(GPIO_OUTPUT_PIN_NUMBER);
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    init_timer();
    init_gpio();
    init_ppi();

    //nrf_timer_event_t event = NRF_TIMER_EVENT_COMPARE2;
    //timer_event_handler(event, NULL);
    start_shit();

    // Loop and increment the timer count value and capture value into LEDs. @note counter is only incremented between TASK_START and TASK_STOP.
    while (true)
    {
        
    }
}


/** @} */
