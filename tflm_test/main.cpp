#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include "LPRTC.hpp"
#include "uart_tracelib.h"
#include "fault_handler.h"
#include <RTE_Components.h>
#include CMSIS_device_header
#include "SEGGER_RTT.h"


static void uart_callback(uint32_t event) { }

static char PRINTF_OUT_STRING[256] __attribute__((used, section(".bss.array_region_sram0")));

int main() {
	fault_dump_enable(true);
	SEGGER_RTT_ConfigUpBuffer(0, nullptr, nullptr, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
	LPRTC::getInstance().enable();

	LPRTC::getInstance().disable();
	while (1) {
		__WFE();
	}

    return 0;
}
