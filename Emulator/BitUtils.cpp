#include "BitUtils.hpp"
#include <sstream>
#include <iomanip>

namespace BitUtils
{
    uint8_t GetMsb(uint16_t byteValue)
    {
        return byteValue >> 8; // (value & 0xFF00) >> 8
    }

    uint8_t GetLsb(uint16_t byteValue)
    {
        return byteValue & 0xFF;
    }

    bool GetBit(uint8_t byteValue, size_t position)
    {
        return byteValue & (1 << position);
    }

    uint8_t SetBit(uint8_t byteValue, size_t position, bool value)
    {
        return value ? SetBit(byteValue, position) : UnsetBit(byteValue, position);
    }

    uint8_t SetBit(uint8_t byteValue, size_t position)
    {
        return byteValue | (1 << position);
    }

    uint8_t UnsetBit(uint8_t byteValue, size_t position)
    {
        return byteValue & ~(1 << position);
    }

    uint8_t Swap(uint8_t byteValue)
    {
        return static_cast<uint8_t>(byteValue << 4 | byteValue >> 4);
    }

    uint16_t ToUnsigned16(uint8_t msb, uint8_t lsb)
    {
        return (static_cast<uint16_t>(msb) << 8) | lsb;
    }

    std::string ToString(uint16_t byteValue)
    {
        std::stringstream ss;
        ss << "0x" << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int)byteValue;
        return ss.str();
    }
}