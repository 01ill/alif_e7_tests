#include <cstddef>
#include <time.h>

#include "RTE_Components.h"
#include CMSIS_device_header

#include <stdio.h>

#include "Driver_GPIO.h"
#include "board.h"
#include "uart_tracelib.h"
#include "fault_handler.h"

#include <chrono>


#define _GET_DRIVER_REF(ref, peri, chan) \
    extern ARM_DRIVER_##peri Driver_##peri##chan; \
    static ARM_DRIVER_##peri * ref = &Driver_##peri##chan;
#define GET_DRIVER_REF(ref, peri, chan) _GET_DRIVER_REF(ref, peri, chan)

GET_DRIVER_REF(gpio_b, GPIO, BOARD_LEDRGB0_B_GPIO_PORT);
GET_DRIVER_REF(gpio_r, GPIO, BOARD_LEDRGB0_R_GPIO_PORT);

extern "C" {
    int run_asm();
    int dotp_scalar(const float * a, const float * b, float * c, int len);
    int dotp_mve(const float * a, const float * b, float * c, int len);
} 

static void uart_callback(uint32_t event) {}

int main (void)
{
//    BOARD_Pinmux_Init();

    tracelib_init(NULL, uart_callback);

    fault_dump_enable(true);

    gpio_b->Initialize(BOARD_LEDRGB0_B_PIN_NO, NULL);
    gpio_b->PowerControl(BOARD_LEDRGB0_B_PIN_NO, ARM_POWER_FULL);
    gpio_b->SetDirection(BOARD_LEDRGB0_B_PIN_NO, GPIO_PIN_DIRECTION_OUTPUT);
    gpio_b->SetValue(BOARD_LEDRGB0_B_PIN_NO, GPIO_PIN_OUTPUT_STATE_LOW);

    gpio_r->Initialize(BOARD_LEDRGB0_R_PIN_NO, NULL);
    gpio_r->PowerControl(BOARD_LEDRGB0_R_PIN_NO, ARM_POWER_FULL);
    gpio_r->SetDirection(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_DIRECTION_OUTPUT);
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_LOW);

#ifdef CORE_M55_HE
    SysTick_Config(SystemCoreClock/10);
#else
    SysTick_Config(SystemCoreClock/25);
#endif

    printf("\r\nHello World!\r\n");
    int val = run_asm();
    printf("\r\nHello %d\r\n", val);

    int len = 16;
    int iterations = 70000;
    float a[len];
    float b[len];
    float c[len];
    for (int i = 0; i < len; i++) {
        a[i] = i;
        b[i] = i;
        c[i] = 0;
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_HIGH);
    for (int i = 0; i < 1000000; i++) {
        dotp_scalar(a, b, c, len);
    }
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_LOW);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double diff = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    printf("C Scalar: %f %f %f %f", c[0], c[1], c[2], c[3]);
    for (int i = 0; i < len; i++) {
        c[i] = 0;
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    begin = std::chrono::steady_clock::now();
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_HIGH);
    for (int i = 0; i < 1000000; i++) {
        dotp_mve(a, b, c, len);
    }
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_LOW);
    end = std::chrono::steady_clock::now();
    diff = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    printf("C Scalar: %f %f %f %f", c[0], c[1], c[2], c[3]);

    while (1) __WFI();
}

void SysTick_Handler (void)
{
#ifdef CORE_M55_HE
    gpio_b->SetValue(BOARD_LEDRGB0_B_PIN_NO, GPIO_PIN_OUTPUT_STATE_TOGGLE);
#else
    gpio_r->SetValue(BOARD_LEDRGB0_R_PIN_NO, GPIO_PIN_OUTPUT_STATE_TOGGLE);
#endif
}

// Stubs to suppress missing stdio definitions for nosys
#define TRAP_RET_ZERO  {__BKPT(0); return 0;}
int _close(int val) TRAP_RET_ZERO
int _lseek(int val0, int val1, int val2) TRAP_RET_ZERO
int _read(int val0, char * val1, int val2) TRAP_RET_ZERO
int _write(int val0, char * val1, int val2) TRAP_RET_ZERO