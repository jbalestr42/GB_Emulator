#pragma once

#include <cstdint>
#include <string>

struct Registers
{
	struct Flags
	{
		// This bit is set when the result of a math operation is zero or
		// two values match when using the CP instruction.
		bool z;
		// This bit is set if a subtraction was performed in the last math instruction.
		bool n;
		// This bit is set if a carry occurred from the lower nibble in the last math operation.
		bool h;
		// This bit is set if a carry occurred from the last math operation or
		// if register A is the smaller value when executing the CP instruction.
		bool c;

		static bool HalfCarry8(uint8_t a, uint8_t b);
		static bool HalfCarry8(uint8_t a, uint8_t b, uint8_t carry);
		static bool HalfCarry16(uint16_t a, uint16_t b);
		static bool HalfCarry16(uint16_t a, uint16_t b, uint16_t carry);
		static bool Carry8(uint8_t a, uint8_t b);
		static bool Carry8(uint8_t a, uint8_t b, uint8_t carry);
		static bool Carry16(uint16_t a, uint16_t b);
		static bool Carry16(uint16_t a, uint16_t b, uint16_t carry);

		static const uint16_t ZERO_FLAG_POS = 7;
		static const uint16_t SUBTRACT_FLAG_POS = 6;
		static const uint16_t HALF_CARRY_FLAG_POS = 5;
		static const uint16_t CARRY_FLAG_POS = 4;
	};

	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t f;
	uint8_t h;
	uint8_t l;
	uint16_t pc;
	uint16_t sp;
	Flags flags;

	uint16_t af();
	void af(uint16_t af);

	uint16_t bc();
	void bc(uint16_t bc);

	uint16_t de();
	void de(uint16_t de);

	uint16_t hl();
	void hl(uint16_t hl);

	std::string ToString();
};