#ifndef LPRTC_REGISTERS_HPP
#define LPRTC_REGISTERS_HPP

#include <cstdint>
#include <type_traits>

namespace LPRTC_Register {
    constexpr uintptr_t LPRTC_BASE_ADDRESS = 0x4200'0000;
    constexpr uintptr_t CCVR_OFFSET = 0x0;
    constexpr uintptr_t CCR_OFFSET = 0xC;
    // HW Reference Manual v2.8, page 211
    constexpr uintptr_t LPRTC_VBAT_BASE_ADDRESS = 0x1A60'9000;
    constexpr uintptr_t LPRTC_RTC_CLK_EN_OFFSET = 0x10;
}

namespace LPRTC_CCR_Bits {
    constexpr uint32_t LPRTC_EN = (1U << 2U);
    constexpr uint32_t LPRTC_WEN = (1U << 3U);
    constexpr uint32_t LPRTC_PSCLR_EN = (1U << 4U);
    // Hardware Reference Manual v2.8, page 214
    constexpr uint32_t LPRTC_CLK_EN = (1U << 0U);
}

template <typename RegisterType, typename AddressType, AddressType Offset>
class Register {
    static_assert(std::is_unsigned_v<AddressType>, "AddressType must be unsigned");
    static_assert(std::is_integral_v<RegisterType>, "RegisterType must be integral");

    protected:
        volatile RegisterType* const reg;

    public:
        explicit Register(AddressType base):
            reg(reinterpret_cast<volatile RegisterType*>(base + Offset)) { }
        
        const RegisterType read() { return *reg; }
        void inline write(RegisterType value) { *reg = value; }
        void inline set(RegisterType mask) { *reg |= mask; }
        void inline clear(RegisterType mask) { *reg &= ~mask; }
        const bool is_set(RegisterType mask) { return  (*reg & mask) != 0; }
};

#endif // LPRTC_REGISTERS_HPP