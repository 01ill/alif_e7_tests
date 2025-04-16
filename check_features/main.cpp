#include "RTE_Components.h"
#include <cstdint>
#include CMSIS_device_header

#include "board.h"

#include "SEGGER_RTT.h"


int main (void)
{
    SEGGER_RTT_ConfigUpBuffer(0, nullptr, nullptr, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

    uint32_t regValue;
    /* CPUID: 0xE000ED00 */
    regValue = *(reinterpret_cast<volatile uint32_t *>(0xe000'ed00));
    SEGGER_RTT_printf(0, "CPUID: %u\n", regValue);
    /* ACTLR: 0xE00E008 */
    regValue = *(reinterpret_cast<volatile uint32_t *>(0xe000'e008));
    SEGGER_RTT_printf(0, "ACTLR: %u\n", regValue);

    /* PFCR: */

    int val = 1;

    while (1) __WFI();
}


// Stubs to suppress missing stdio definitions for nosys
#define TRAP_RET_ZERO  {__BKPT(0); return 0;}
int _close(int val) TRAP_RET_ZERO
int _lseek(int val0, int val1, int val2) TRAP_RET_ZERO
int _read(int val0, char * val1, int val2) TRAP_RET_ZERO
int _write(int val0, char * val1, int val2) TRAP_RET_ZERO