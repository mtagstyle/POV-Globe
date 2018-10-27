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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "app_uart.h"
#include "app_error.h"
#include "boards.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
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
nrf_drv_timer_config_t timer_config = {NRF_TIMER_FREQ_16MHz, NRF_TIMER_MODE_TIMER, NRF_TIMER_BIT_WIDTH_32, 1, NULL};

const nrf_drv_timer_t timer0 = NRF_DRV_TIMER_INSTANCE(0);
const nrf_drv_timer_t timer1 = NRF_DRV_TIMER_INSTANCE(1);
const nrf_drv_timer_t timer2 = NRF_DRV_TIMER_INSTANCE(2);

nrf_ppi_channel_t ppi_channel1, ppi_channel2;

// Timer even handler. Not used since timer is used only for PPI.
void timer_event_handler(nrf_timer_event_t event_type, void * p_context){}

static void init()
{
    // Initialize PPI peripheral
    uint32_t error = NRF_SUCCESS;
    error = nrf_drv_ppi_init();
    APP_ERROR_CHECK(error);
    // Initialize timers
    error = nrf_drv_timer_init(&timer0, &timer_config, timer_event_handler);
    APP_ERROR_CHECK(error);
    error = nrf_drv_timer_init(&timer1, &timer_config, timer_event_handler);
    APP_ERROR_CHECK(error);
    error = nrf_drv_timer_init(&timer2, &timer_config, timer_event_handler);
    APP_ERROR_CHECK(error);

    // Timer 0, cutoff should be 0.35 us
    nrf_drv_timer_extended_compare(&timer0, NRF_TIMER_CC_CHANNEL0, TIMER0_COMPARE_VALUE, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);
    APP_ERROR_CHECK(error);

    // Timer 1, cutoff should be 0.9 us
    nrf_drv_timer_extended_compare(&timer1, NRF_TIMER_CC_CHANNEL1, TIMER1_COMPARE_VALUE, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, false);
    APP_ERROR_CHECK(error);

    // Timer 2, cutoff should be 1.25 us, interrupt here since it is the end of the sequence
    nrf_drv_timer_extended_compare(&timer2, NRF_TIMER_CC_CHANNEL2, TIMER2_COMPARE_VALUE, NRF_TIMER_SHORT_COMPARE2_CLEAR_MASK, true);
    APP_ERROR_CHECK(error);
}

/** @brief Function for initializing the PPI peripheral.
*/
/*
static void ppi_init(void)
{
    uint32_t err_code = NRF_SUCCESS;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    // Configure 1st available PPI channel to stop TIMER0 counter on TIMER1 COMPARE[0] match, which is every even number of seconds.
    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel1);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_assign(ppi_channel1,
                                          nrf_drv_timer_event_address_get(&timer1, NRF_TIMER_EVENT_COMPARE0),
                                          nrf_drv_timer_task_address_get(&timer0, NRF_TIMER_TASK_STOP));
    APP_ERROR_CHECK(err_code);

    // Configure 2nd available PPI channel to start timer0 counter at TIMER2 COMPARE[0] match, which is every odd number of seconds.
    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel2);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_assign(ppi_channel2,
                                          nrf_drv_timer_event_address_get(&timer2, NRF_TIMER_EVENT_COMPARE0),
                                          nrf_drv_timer_task_address_get(&timer0, NRF_TIMER_TASK_START));
    APP_ERROR_CHECK(err_code);

    // Enable both configured PPI channels
    err_code = nrf_drv_ppi_channel_enable(ppi_channel1);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_enable(ppi_channel2);
    APP_ERROR_CHECK(err_code);
}
*/
/**
 * @brief Function for application main entry.
 */
int main(void)
{
    init();
    //ppi_init();    // PPI to redirect the event to timer start/stop tasks.

    // Enabling constant latency as indicated by PAN 11 "HFCLK: Base current with HFCLK 
    // running is too high" found at Product Anomaly document found at
    // https://www.nordicsemi.com/eng/Products/Bluetooth-R-low-energy/nRF51822/#Downloads
    //
    // @note This example does not go to low power mode therefore constant latency is not needed.
    //       However this setting will ensure correct behaviour when routing TIMER events through 
    //       PPI (shown in this example) and low power mode simultaneously.
    NRF_POWER->TASKS_CONSTLAT = 1;
    
    // Start clock.
    nrf_drv_timer_enable(&timer0);
    nrf_drv_timer_enable(&timer1);
    nrf_drv_timer_enable(&timer2);

    // Loop and increment the timer count value and capture value into LEDs. @note counter is only incremented between TASK_START and TASK_STOP.
    while (true)
    {
        printf("Current cout: %d\n\r", (int)nrf_drv_timer_capture(&timer0,NRF_TIMER_CC_CHANNEL0));

        /* increment the counter */
        nrf_drv_timer_increment(&timer0);

        nrf_delay_ms(100);
    }
}


/** @} */