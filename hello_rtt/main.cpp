/* Copyright (C) 2024 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
*/
#include <chrono>
#include <cstdint>
#include <stdio.h>

#include "timing.h"
#include "Driver_GPIO.h"
#include "pinconf.h"
#include "board.h"
#include "RTE_Components.h"
#include CMSIS_device_header

#include "uart_tracelib.h"
#include "fault_handler.h"

#define _GET_DRIVER_REF(ref, peri, chan) extern ARM_DRIVER_##peri Driver_##peri##chan; static ARM_DRIVER_##peri * ref = &Driver_##peri##chan;
#define GET_DRIVER_REF(ref, peri, chan) _GET_DRIVER_REF(ref, peri, chan)

GET_DRIVER_REF(gpio_b, GPIO, BOARD_LEDRGB0_B_GPIO_PORT);
GET_DRIVER_REF(gpio_r, GPIO, BOARD_LEDRGB0_R_GPIO_PORT);


static void uart_callback(uint32_t event)
{ }

extern "C" {
    int dotp_scalar(const float * a, const float * b, float * c, int len);
    int dotp_mve(const float * a, const float * b, float * c, int len);
}

//extern int dotp_scalar(const float * a, const float * b, float * c, int len);
//extern int dotp_mve(const float * a, const float * b, float * c, int len);

int main (void)
{
    // Init pinmux using boardlib
    // BOARD_Pinmux_Init();

    fault_dump_enable(true);

    // SysTick_Config(SystemCoreClock / 10);

    RTC_Initialize();
    
    gpio_b->Initialize(BOARD_LEDRGB0_B_PIN_NO, NULL);
    gpio_b->PowerControl(BOARD_LEDRGB0_B_PIN_NO, ARM_POWER_FULL);
    gpio_b->SetDirection(BOARD_LEDRGB0_B_PIN_NO, GPIO_PIN_DIRECTION_OUTPUT);
    gpio_b->SetValue(BOARD_LEDRGB0_B_PIN_NO, GPIO_PIN_OUTPUT_STATE_LOW);

    gpio_r->Initialize(BOARD_LEDRGB0_R_PIN_NO, NULL);
    gpio_r->PowerControl(BOARD_LEDRGB0_R_PIN_NO, ARM_POWER_FULL);
    gpio_r->SetDirection(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_DIRECTION_OUTPUT);
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_LOW);


    printf("\r\nHello World!\r\n");

    int len = 16;
    int iterations = 1000;
    float a[len];
    float b[len];
    float c[len];
    for (int i = 0; i < len; i++) {
        a[i] = i;
        b[i] = i;
        c[i] = 0;
    }


    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_HIGH);
    std::chrono::time_point start = RTC_Clock::now();
    for (int i = 0; i < iterations; i++) {
        dotp_scalar(a, b, c, len);
    }
    std::chrono::time_point end = RTC_Clock::now();
    int32_t diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_LOW);
    printf("C Scalar (%d): %f %f %f %f \r\n", diff, c[0], c[1], c[2], c[3]);
    for (int i = 0; i < len; i++) {
        c[i] = 0;
    }
    RTC_Sleep(1000);
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_HIGH);
    start = RTC_Clock::now();
    for (int i = 0; i < iterations; i++) {
        dotp_mve(a, b, c, len);
    }
    end = RTC_Clock::now();
    diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_LOW);
    printf("C MVE (%d): %f %f %f %f \r\n", diff, c[0], c[1], c[2], c[3]);

    RTC_Uninitialize();

    while (1) __WFE();
}


#define TRAP_RET_ZERO  {__BKPT(0); return 0;}
int _close(int val) TRAP_RET_ZERO
int _lseek(int val0, int val1, int val2) TRAP_RET_ZERO
int _read(int val0, char * val1, int val2) TRAP_RET_ZERO
int _write(int val0, char * val1, int val2) TRAP_RET_ZERO