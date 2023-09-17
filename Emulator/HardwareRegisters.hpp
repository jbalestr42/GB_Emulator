#pragma once

#include <cstdint>

namespace HardwareRegisters
{
	static const uint16_t SB_ADDR = 0xFF01; // Serial transfer data
	static const uint16_t SC_ADDR = 0xFF02; // Serial transfer control
	static const uint16_t DIV_ADDR = 0xFF04; // Divider register
	static const uint16_t TIMA_ADDR = 0xFF05; // Timer counter
	static const uint16_t TMA_ADDR = 0xFF06; // Timer modulo
	static const uint16_t TAC_ADDR = 0xFF07; // Timer control
	static const uint16_t IF_ADDR = 0xFF0F; // Interrupt flag
	static const uint16_t LY_ADDR = 0xFF44; // LCD Y coordinate
	static const uint16_t IE_ADDR = 0xFFFF; // Interrupt enable
}