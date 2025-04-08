#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include "LPRTC.hpp"
#include "m-profile/cmsis_armclang_m.h"
#include "uart_tracelib.h"
#include "fault_handler.h"
#include <RTE_Components.h>
#include CMSIS_device_header
#include "SEGGER_RTT.h"


static void uart_callback(uint32_t event) { }

static char PRINTF_OUT_STRING[256] __attribute__((used, section(".bss.array_region_sram0")));

__NO_RETURN int main() {
	fault_dump_enable(true);
	SEGGER_RTT_ConfigUpBuffer(0, nullptr, nullptr, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
	LPRTC::getInstance().enable();


	//using jitTest = void;

	typedef void (*jitTest)(void);
	// uint32_t (* jitTest)() = void;

	static const uint32_t instr[] = { 0x4770 };
	__DSB(); // Data Synchronization Barrier
	__ISB(); // Instruction Synchronization Barrier

	jitTest jt = (jitTest)((unsigned)instr | 0x1);
	jt();

	LPRTC::getInstance().disable();
	while (1) {
		__WFE();
	}
}
