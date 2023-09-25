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
	static const uint16_t LCDC_ADDR = 0xFF40; // LCD control
	static const uint16_t STAT_ADDR = 0xFF41; // LCD status
	static const uint16_t SCY_ADDR = 0xFF42; // Viewport Y position
	static const uint16_t SCX_ADDR = 0xFF43; // Viewport X position
	static const uint16_t LY_ADDR = 0xFF44; // LCD Y coordinate
	static const uint16_t LYC_ADDR = 0xFF45; // LY compare
	static const uint16_t DMA_ADDR = 0xFF46; // OAM DMA source address & start
	static const uint16_t BGP_ADDR = 0xFF47; // BG palette data
	static const uint16_t OBP0_ADDR = 0xFF48; // OBJ palette 0 data
	static const uint16_t OBP1_ADDR = 0xFF49; // OBJ palette 1 data
	static const uint16_t WY_ADDR = 0xFF4A; // Window Y position
	static const uint16_t WX_ADDR = 0xFF4B; // Window X position plus 7
	static const uint16_t IE_ADDR = 0xFFFF; // Interrupt enable
}