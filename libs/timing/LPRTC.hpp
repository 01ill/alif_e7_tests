#ifndef LPRTC_HPP
#define LPRTC_HPP

#include <cstdint>
#include "LPRTC_Registers.hpp"


class LPRTC {
    private:
        static inline Register<uint32_t, uintptr_t, LPRTC_Register::LPRTC_RTC_CLK_EN_OFFSET> RTC_CLK_EN{LPRTC_Register::LPRTC_VBAT_BASE_ADDRESS};
        static inline Register<uint32_t, uintptr_t, LPRTC_Register::CCVR_OFFSET> CCVR{LPRTC_Register::LPRTC_BASE_ADDRESS};
        static inline Register<uint32_t, uintptr_t, LPRTC_Register::CCR_OFFSET> CCR{LPRTC_Register::LPRTC_BASE_ADDRESS};

    public:
        LPRTC() = default;
        LPRTC(const LPRTC&) = delete;
        LPRTC& operator=(const LPRTC&) = delete;

        static LPRTC& getInstance() {
            static LPRTC instance;
            return instance;
        }

        void inline enable() {
            RTC_CLK_EN.set(LPRTC_CCR_Bits::LPRTC_CLK_EN);
            CCR.set(LPRTC_CCR_Bits::LPRTC_EN);
        }
        
        void inline disable() {
            CCR.clear(LPRTC_CCR_Bits::LPRTC_EN);
            RTC_CLK_EN.clear(LPRTC_CCR_Bits::LPRTC_CLK_EN);
        }
        
        void inline enablePrescaler() {
            CCR.set(LPRTC_CCR_Bits::LPRTC_PSCLR_EN);
        }
        
        void inline disablePrescaler() {
            CCR.clear(LPRTC_CCR_Bits::LPRTC_PSCLR_EN);
        }
        
        uint32_t inline getCurrentValue() const {
            return CCVR.read();
        }
};

static inline void LPRTC_Enable() {
    LPRTC::getInstance().enable();
}

static inline uint32_t LPRTC_GetValue() {
    return LPRTC::getInstance().getCurrentValue();
}

#endif // LPRTC_HPP