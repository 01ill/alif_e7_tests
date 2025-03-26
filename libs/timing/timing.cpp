#include <cstdint>
extern "C" {
    #include "Driver_Common.h"
    #include "Driver_RTC.h"
}

#include "timing.hpp"

/*
Siehe: CMSIS Ensemble Pack -> Boards -> E7 -> Templates -> Baremetal -> RTC_Baremetal
*/
// RTC0 ist die Standard-RTC0 Clock
// Ist in der RTE_Device.h (RTE/Device/*/RTE_Device.h) zu finden
// Treiber ist im CMSIS-Pack der Driver_RTC.c
extern ARM_DRIVER_RTC Driver_RTC0;
static ARM_DRIVER_RTC *RTCdrv = &Driver_RTC0;

bool powerOn() {
    int32_t ret = RTCdrv->PowerControl(ARM_POWER_FULL);
    return ret == ARM_DRIVER_OK;
}

bool powerOff() {
    int32_t ret = RTCdrv->PowerControl(ARM_POWER_OFF);
    return ret == ARM_DRIVER_OK;
}

bool disablePrescaler() {
    // Prescaler auf 1 setzen, damit wird er quasi deaktiviert
    return RTCdrv->Control(ARM_RTC_SET_PRESCALER, 1);
}

/**
 * Vorgehen: Initialisierung -> PowerOn -> Prescaler deaktiviern -> Counter abfragen x-mal -> PowerOff -> Uninitialize
*/
bool RTC_Initialize() {
    int32_t ret = RTCdrv->Initialize(NULL);
    if (ret != ARM_DRIVER_OK) {
        return false;
    }
    ret = RTCdrv->PowerControl(ARM_POWER_FULL);
    if (ret != ARM_DRIVER_OK) {
        RTC_Uninitialize();
        return false;
    }
    ret = disablePrescaler();
    if (ret != ARM_DRIVER_OK) {
        RTC_Uninitialize();
        return false;
    }

    return true;
}

bool RTC_Uninitialize() {
    powerOff();
    int32_t ret = RTCdrv->Uninitialize();
    return ret == ARM_DRIVER_OK;
}

uint32_t RTC_GetTimepoint() {
    uint32_t val;
    int32_t ret = RTCdrv->ReadCounter(&val);
    return ret == ARM_DRIVER_OK ? val / 3277 : 0;
}

void RTC_Sleep(uint32_t ms) {
    uint32_t start = RTC_GetTimepoint();
    while (RTC_GetTimepoint() - start < ms) { }
}