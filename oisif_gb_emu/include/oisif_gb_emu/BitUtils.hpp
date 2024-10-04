#pragma once

#include <cstdint>
#include <string>

namespace BitUtils
{
	uint8_t GetMsb(uint16_t byteValue);
	uint8_t GetLsb(uint16_t byteValue);
	bool GetBit(uint8_t byteValue, size_t position);
	uint8_t SetBit(uint8_t byteValue, size_t position, bool value);
	uint8_t SetBit(uint8_t byteValue, size_t position);
	uint8_t UnsetBit(uint8_t byteValue, size_t position);
	uint8_t Swap(uint8_t byteValue);
	uint16_t ToUnsigned16(uint8_t msb, uint8_t lsb);
	std::string ToString(uint16_t byteValue);
}