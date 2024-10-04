#include "Registers.hpp"
#include "BitUtils.hpp"
#include <sstream>
#include <iomanip>

uint16_t Registers::af()
{
	f = flags.z << Flags::ZERO_FLAG_POS
		| flags.n << Flags::SUBTRACT_FLAG_POS
		| flags.h << Flags::HALF_CARRY_FLAG_POS
		| flags.c << Flags::CARRY_FLAG_POS;
	return BitUtils::ToUnsigned16(a, f);
}

void Registers::af(uint16_t af)
{
	a = BitUtils::GetMsb(af);
	f = BitUtils::GetLsb(af);
	flags.z = BitUtils::GetBit(f, Flags::ZERO_FLAG_POS);
	flags.n = BitUtils::GetBit(f, Flags::SUBTRACT_FLAG_POS);
	flags.h = BitUtils::GetBit(f, Flags::HALF_CARRY_FLAG_POS);
	flags.c = BitUtils::GetBit(f, Flags::CARRY_FLAG_POS);
}

uint16_t Registers::bc()
{
	return BitUtils::ToUnsigned16(b, c);
}

void Registers::bc(uint16_t bc)
{
	b = BitUtils::GetMsb(bc);
	c = BitUtils::GetLsb(bc);
}

uint16_t Registers::de()
{
	return BitUtils::ToUnsigned16(d, e);
}

void Registers::de(uint16_t de)
{
	d = BitUtils::GetMsb(de);
	e = BitUtils::GetLsb(de);
}

uint16_t Registers::hl()
{
	return BitUtils::ToUnsigned16(h, l);
}

void Registers::hl(uint16_t hl)
{
	h = BitUtils::GetMsb(hl);
	l = BitUtils::GetLsb(hl);
}

std::string Registers::ToString()
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::uppercase;
	ss << "A: " << std::setw(2) << static_cast<int>(a) << " ";
	ss << "F: " << std::setw(2) << static_cast<int>(flags.z << Flags::ZERO_FLAG_POS
		| flags.n << Flags::SUBTRACT_FLAG_POS
		| flags.h << Flags::HALF_CARRY_FLAG_POS
		| flags.c << Flags::CARRY_FLAG_POS) << " ";
	ss << "B: " << std::setw(2) << static_cast<int>(b) << " ";
	ss << "C: " << std::setw(2) << static_cast<int>(c) << " ";
	ss << "D: " << std::setw(2) << static_cast<int>(d) << " ";
	ss << "E: " << std::setw(2) << static_cast<int>(e) << " ";
	ss << "H: " << std::setw(2) << static_cast<int>(h) << " ";
	ss << "L: " << std::setw(2) << static_cast<int>(l) << " ";
	//ss << "af: " << std::setw(2) << af() << " | ";
	//ss << "bc: " << std::setw(2) << bc() << " | ";
	//ss << "de: " << std::setw(2) << de() << " | ";
	//ss << "hl: " << std::setw(2) << hl() << " | ";
	ss << "SP: " << std::setw(4) << sp << " ";
	ss << "PC: 00:" << std::setw(4) << pc;
	return ss.str();
}

bool Registers::Flags::HalfCarry8(uint8_t a, uint8_t b)
{
	return ((a & 0xF) + (b & 0xF)) >= 0x10;
}

bool Registers::Flags::HalfCarry8(uint8_t a, uint8_t b, uint8_t carry)
{
	return ((a & 0xF) + (b & 0xF) + (carry & 0xF)) >= 0x10;
}

bool Registers::Flags::HalfCarry16(uint16_t a, uint16_t b)
{
	return ((a & 0xFFF) + (b & 0xFFF)) >= 0x1000;
}

bool Registers::Flags::HalfCarry16(uint16_t a, uint16_t b, uint16_t carry)
{
	return ((a & 0xFFF) + (b & 0xFFF) + (carry & 0xFFF)) >= 0x1000;
}

bool Registers::Flags::Carry8(uint8_t a, uint8_t b)
{
	return (static_cast<uint16_t>(a) + static_cast<uint16_t>(b)) >= 0x100;
}

bool Registers::Flags::Carry8(uint8_t a, uint8_t b, uint8_t carry)
{
	return (static_cast<uint16_t>(a) + static_cast<uint16_t>(b) + static_cast<uint16_t>(carry)) >= 0x100;
}

bool Registers::Flags::Carry16(uint16_t a, uint16_t b)
{
	return (static_cast<uint32_t>(a) + static_cast<uint32_t>(b)) >= 0x10000;
}

bool Registers::Flags::Carry16(uint16_t a, uint16_t b, uint16_t carry)
{
	return (static_cast<uint32_t>(a) + static_cast<uint32_t>(b) + static_cast<uint32_t>(carry)) >= 0x10000;
}
