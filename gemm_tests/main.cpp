/* Copyright (C) 2024 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
*/
// #include <arm_mve_types.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>

#include "board.h"
#include "RTE_Components.h"
#include CMSIS_device_header

#include "uart_tracelib.h"
#include "fault_handler.h"

#include "benchmark.hpp"
#include "timing.hpp"
#include <arm_mve.h>
#include "arm_math.h"

static void uart_callback(uint32_t event)
{ }

static constexpr uint32_t arrayMaxSize = 32;
static float32_t bigA[arrayMaxSize*arrayMaxSize]; //__attribute__((used, section(".bss.array_region_sram0")));
static float32_t bigB[arrayMaxSize*arrayMaxSize]; //__attribute__((used, section(".bss.array_region_sram0")));
static float32_t bigC[arrayMaxSize*arrayMaxSize]; //__attribute__((used, section(".bss.array_region_sram0")));

extern "C" {
    float32_t gemm_4x6(const float32_t * a, const float32_t * b, float32_t * c, const uint32_t len_k);
    float32_t gemm_4x4(const float32_t * a, const float32_t * b, float32_t * c, const uint32_t len_k);
}

float32_t gemm_reference_4x6(const float32_t *__restrict__ a, const float32_t *__restrict__ b, float32_t * __restrict__ c, const uint32_t len_k) {
    const uint32_t len_n = 6;
    const uint32_t len_m = 4;
    for (uint32_t k = 0; k < len_k; k++) {
        for (uint32_t n = 0; n < len_n; n++) {
            for (uint32_t m = 0; m < len_m; m++) {
                c[m * len_n + n] += a[m * len_k + k] * b[k * len_n + n];
            }
        }    
    }
    return c[0];
}

float32_t gemm_reference_4x4(const float32_t *__restrict__ a, const float32_t *__restrict__ b, float32_t * __restrict__ c, const uint32_t len_k) {
    const uint32_t len_n = 4;
    const uint32_t len_m = 4;
    for (uint32_t k = 0; k < len_k; k++) {
        for (uint32_t n = 0; n < len_n; n++) {
            for (uint32_t m = 0; m < len_m; m++) {
                c[m * len_n + n] += a[m * len_k + k] * b[k * len_n + n];
            }
        }    
    }
    return c[0];
}


int main (void)
{
    fault_dump_enable(true);

    setupTests();

    const uint32_t len_k = 4;
    uint32_t iterations = 10000;

    float32_t a[4 * len_k] = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    }; // m*k

    float32_t b[4 * len_k] = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    }; // n*k

    float32_t c[4 * 4]; // m*n
    float32_t result;

    /*

    for (int32_t i = 0; i < 4*len_k; i++) {
        a[i] = i;
    }

    for (int32_t i = 0; i < 6*len_k; i++) {
        b[i] = i;
    }
*/
    for (int32_t i = 0; i < 4*4; i++) {
        c[i] = 0;
    }

    uint32_t time;
    float32_t gflops;
    time = benchmark(gemm_reference_4x4, iterations, &result, a, b, c, len_k);
    gflops = ((iterations * 2 * pow(len_k, 3) * 1000.0f)  / time) / 1000000000.0f;
    printf("C Reference 4x4 (%d): %f, %f, %f\r\n", time, c[0], result, gflops);
    for (int32_t i = 0; i < 4*4; i++) {
        c[i] = i;
    }
    RTC_Sleep(500);
    time = benchmark(gemm_4x4, iterations, &result, a, b, c, len_k);
    gflops = ((iterations  * (8 * 4 * 4 + 8) * 1000.0f)  / time) / pow(10, 9);
    printf("ASM 4x4 (%d): %f, %f, %f\r\n", time, c[0], result, gflops);
    
    arm_matrix_instance_f32 armA;
    arm_matrix_instance_f32 armB;
    arm_matrix_instance_f32 armC;
    arm_status status;

    for (uint32_t i = 4; i <= arrayMaxSize; i = i * 2) {
        for (uint32_t j = 0; j < i*i; j++) {
            bigA[j] = j;
            bigB[j] = j;
            bigC[j] = 0;
        }
        arm_mat_init_f32(&armA, i, i, bigA);
        arm_mat_init_f32(&armB, i, i, bigB);
        arm_mat_init_f32(&armC, i, i, bigC);

        time = benchmarkArm(arm_mat_mult_f32, iterations, &status, &armA, &armB, &armC);
        gflops = ((iterations  * pow(i,3) * 1000.0f)  / time) / pow(10, 9);
        printf("CMSIS-DSP %dx%d (%d): %f, %f, %f\r\n", i, i, time, c[0], result, gflops);
        iterations = iterations / 2;
    }
    printf("Fertig!");

    stopTests();

    while (1) __WFE();
}

/*
#define TRAP_RET_ZERO  {__BKPT(0); return 0;}
int _close(int val) TRAP_RET_ZERO
int _lseek(int val0, int val1, int val2) TRAP_RET_ZERO
int _read(int val0, char * val1, int val2) TRAP_RET_ZERO
int _write(int val0, char * val1, int val2) TRAP_RET_ZERO
*/