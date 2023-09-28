#include "CPU.hpp"
#include <iostream>

void CPU::initInstructions()
{
	// LD r,n
	{
		_instructions[0x06] = OpCode("LD B,n", 0x06, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.b = _data.lsb; } });
		_instructions[0x0E] = OpCode("LD C,n", 0x0E, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.c = _data.lsb; } });
		_instructions[0x16] = OpCode("LD D,n", 0x16, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.d = _data.lsb; } });
		_instructions[0x1E] = OpCode("LD E,n", 0x1E, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.e = _data.lsb; } });
		_instructions[0x26] = OpCode("LD H,n", 0x26, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.h = _data.lsb; } });
		_instructions[0x2E] = OpCode("LD L,n", 0x2E, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.l = _data.lsb; } });
	}

	// LD r1,r2
	{
		_instructions[0x40] = OpCode("LD B,B", 0x40, 1, 4, {
			[&]() { _registers.b = _registers.b; } });
		_instructions[0x41] = OpCode("LD B,C", 0x41, 1, 4, {
			[&]() { _registers.b = _registers.c; } });
		_instructions[0x42] = OpCode("LD B,D", 0x42, 1, 4, {
			[&]() { _registers.b = _registers.d; } });
		_instructions[0x43] = OpCode("LD B,E", 0x43, 1, 4, {
			[&]() { _registers.b = _registers.e; } });
		_instructions[0x44] = OpCode("LD B,H", 0x44, 1, 4, {
			[&]() { _registers.b = _registers.h; } });
		_instructions[0x45] = OpCode("LD B,L", 0x45, 1, 4, {
			[&]() { _registers.b = _registers.l; } });
		_instructions[0x46] = OpCode("LD B,(HL)", 0x46, 1, 8, {
			[&]() { _data.addr = _registers.hl(); },
			[&]() { _registers.b = _mmu.read8(_data.addr); } });

		_instructions[0x48] = OpCode("LD C,B", 0x48, 1, 4, {
			[&]() { _registers.c = _registers.b; } });
		_instructions[0x49] = OpCode("LD C,C", 0x49, 1, 4, {
			[&]() { _registers.c = _registers.c; } });
		_instructions[0x4A] = OpCode("LD C,D", 0x4A, 1, 4, {
			[&]() { _registers.c = _registers.d; } });
		_instructions[0x4B] = OpCode("LD C,E", 0x4B, 1, 4, {
			[&]() { _registers.c = _registers.e; } });
		_instructions[0x4C] = OpCode("LD C,H", 0x4C, 1, 4, {
			[&]() { _registers.c = _registers.h; } });
		_instructions[0x4D] = OpCode("LD C,L", 0x4D, 1, 4, {
			[&]() { _registers.c = _registers.l; } });
		_instructions[0x4E] = OpCode("LD C,(HL)", 0x4E, 1, 8, {
			[&]() { _data.addr = _registers.hl(); },
			[&]() { _registers.c = _mmu.read8(_data.addr); } });

		_instructions[0x50] = OpCode("LD D,B", 0x50, 1, 4, {
			[&]() { _registers.d = _registers.b; } });
		_instructions[0x51] = OpCode("LD D,C", 0x51, 1, 4, {
			[&]() { _registers.d = _registers.c; } });
		_instructions[0x52] = OpCode("LD D,D", 0x52, 1, 4, {
			[&]() { _registers.d = _registers.d; } });
		_instructions[0x53] = OpCode("LD D,E", 0x53, 1, 4, {
			[&]() { _registers.d = _registers.e; } });
		_instructions[0x54] = OpCode("LD D,H", 0x54, 1, 4, {
			[&]() { _registers.d = _registers.h; } });
		_instructions[0x55] = OpCode("LD D,L", 0x55, 1, 4, {
			[&]() { _registers.d = _registers.l; } });
		_instructions[0x56] = OpCode("LD D,(HL)", 0x56, 1, 8, {
			[&]() { _data.addr = _registers.hl(); },
			[&]() { _registers.d = _mmu.read8(_data.addr); } });

		_instructions[0x58] = OpCode("LD E,B", 0x58, 1, 4, {
			[&]() { _registers.e = _registers.b; } });
		_instructions[0x59] = OpCode("LD E,C", 0x59, 1, 4, {
			[&]() { _registers.e = _registers.c; } });
		_instructions[0x5A] = OpCode("LD E,D", 0x5A, 1, 4, {
			[&]() { _registers.e = _registers.d; } });
		_instructions[0x5B] = OpCode("LD E,E", 0x5B, 1, 4, {
			[&]() { _registers.e = _registers.e; } });
		_instructions[0x5C] = OpCode("LD E,H", 0x5C, 1, 4, {
			[&]() { _registers.e = _registers.h; } });
		_instructions[0x5D] = OpCode("LD E,L", 0x5D, 1, 4, {
			[&]() { _registers.e = _registers.l; } });
		_instructions[0x5E] = OpCode("LD E,(HL)", 0x5E, 1, 8, {
			[&]() { _data.addr = _registers.hl(); },
			[&]() { _registers.e = _mmu.read8(_data.addr); } });

		_instructions[0x60] = OpCode("LD H,B", 0x60, 1, 4, {
			[&]() { _registers.h = _registers.b; } });
		_instructions[0x61] = OpCode("LD H,C", 0x61, 1, 4, {
			[&]() { _registers.h = _registers.c; } });
		_instructions[0x62] = OpCode("LD H,D", 0x62, 1, 4, {
			[&]() { _registers.h = _registers.d; } });
		_instructions[0x63] = OpCode("LD H,E", 0x63, 1, 4, {
			[&]() { _registers.h = _registers.e; } });
		_instructions[0x64] = OpCode("LD H,H", 0x64, 1, 4, {
			[&]() { _registers.h = _registers.h; } });
		_instructions[0x65] = OpCode("LD H,L", 0x65, 1, 4, {
			[&]() { _registers.h = _registers.l; } });
		_instructions[0x66] = OpCode("LD H,(HL)", 0x66, 1, 8, {
			[&]() { _data.addr = _registers.hl(); },
			[&]() { _registers.h = _mmu.read8(_data.addr); } });

		_instructions[0x68] = OpCode("LD L,B", 0x68, 1, 4, {
			[&]() { _registers.l = _registers.b; } });
		_instructions[0x69] = OpCode("LD L,C", 0x69, 1, 4, {
			[&]() { _registers.l = _registers.c; } });
		_instructions[0x6A] = OpCode("LD L,D", 0x6A, 1, 4, {
			[&]() { _registers.l = _registers.d; } });
		_instructions[0x6B] = OpCode("LD L,E", 0x6B, 1, 4, {
			[&]() { _registers.l = _registers.e; } });
		_instructions[0x6C] = OpCode("LD L,H", 0x6C, 1, 4, {
			[&]() { _registers.l = _registers.h; } });
		_instructions[0x6D] = OpCode("LD L,L", 0x6D, 1, 4, {
			[&]() { _registers.l = _registers.l; } });
		_instructions[0x6E] = OpCode("LD L,(HL)", 0x6E, 1, 8, {
			[&]() { _data.addr = _registers.hl(); },
			[&]() { _registers.l = _mmu.read8(_data.addr); } });

		_instructions[0x70] = OpCode("LD (HL),B", 0x70, 1, 8, {
			[&]() { _data.lsb = _registers.b; },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		_instructions[0x71] = OpCode("LD (HL),C", 0x71, 1, 8, {
			[&]() { _data.lsb = _registers.c; },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		_instructions[0x72] = OpCode("LD (HL),D", 0x72, 1, 8, {
			[&]() { _data.lsb = _registers.d; },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		_instructions[0x73] = OpCode("LD (HL),E", 0x73, 1, 8, {
			[&]() { _data.lsb = _registers.e; },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		_instructions[0x74] = OpCode("LD (HL),H", 0x74, 1, 8, {
			[&]() { _data.lsb = _registers.h; },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		_instructions[0x75] = OpCode("LD (HL),L", 0x75, 1, 8, {
			[&]() { _data.lsb = _registers.l; },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		_instructions[0x36] = OpCode("LD (HL),n", 0x36, 2, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// LD A,r
	{
		_instructions[0x7F] = OpCode("LD A,A", 0x7F, 1, 4, {
			[&]() { _registers.a = _registers.a; } });
		_instructions[0x78] = OpCode("LD A,B", 0x78, 1, 4, {
			[&]() { _registers.a = _registers.b; } });
		_instructions[0x79] = OpCode("LD A,C", 0x79, 1, 4, {
			[&]() { _registers.a = _registers.c; } });
		_instructions[0x7A] = OpCode("LD A,D", 0x7A, 1, 4, {
			[&]() { _registers.a = _registers.d; } });
		_instructions[0x7B] = OpCode("LD A,E", 0x7B, 1, 4, {
			[&]() { _registers.a = _registers.e; } });
		_instructions[0x7C] = OpCode("LD A,H", 0x7C, 1, 4, {
			[&]() { _registers.a = _registers.h; } });
		_instructions[0x7D] = OpCode("LD A,L", 0x7D, 1, 4, {
			[&]() { _registers.a = _registers.l; } });
		_instructions[0x0A] = OpCode("LD A,(BC)", 0x0A, 1, 8, {
			[&]() { _data.addr = _registers.bc(); },
			[&]() { _registers.a = _mmu.read8(_data.addr); } });
		_instructions[0x1A] = OpCode("LD A,(DE)", 0x1A, 1, 8, {
			[&]() { _data.addr = _registers.de(); },
			[&]() { _registers.a = _mmu.read8(_data.addr); } });
		_instructions[0x7E] = OpCode("LD A,(HL)", 0x7E, 1, 8, {
			[&]() { _data.addr = _registers.hl(); },
			[&]() { _registers.a = _mmu.read8(_data.addr); } });
		_instructions[0xFA] = OpCode("LD A,(nn)", 0xFA, 3, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _data.addr = BitUtils::ToUnsigned16(_data.msb, _data.lsb); },
			[&]() { _registers.a = _mmu.read8(_data.addr); } });
		_instructions[0x3E] = OpCode("LD A,n", 0x3E, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.a = _data.lsb; } });
	}

	// LD r,A
	{
		_instructions[0x47] = OpCode("LD B,A", 0x47, 1, 4, {
			[&]() { _registers.b = _registers.a; } });
		_instructions[0x4F] = OpCode("LD C,A", 0x4F, 1, 4, {
			[&]() { _registers.c = _registers.a; } });
		_instructions[0x57] = OpCode("LD D,A", 0x57, 1, 4, {
			[&]() { _registers.d = _registers.a; } });
		_instructions[0x5F] = OpCode("LD E,A", 0x5F, 1, 4, {
			[&]() { _registers.e = _registers.a; } });
		_instructions[0x67] = OpCode("LD H,A", 0x67, 1, 4, {
			[&]() { _registers.h = _registers.a; } });
		_instructions[0x6F] = OpCode("LD L,A", 0x6F, 1, 4, {
			[&]() { _registers.l = _registers.a; } });
		_instructions[0x02] = OpCode("LD (BC),A", 0x02, 1, 8, {
			[&]() { _data.lsb = _registers.a; },
			[&]() { _mmu.write8(_registers.bc(), _data.lsb); } });
		_instructions[0x12] = OpCode("LD (DE),A", 0x12, 1, 8, {
			[&]() { _data.lsb = _registers.a; },
			[&]() { _mmu.write8(_registers.de(), _data.lsb); } });
		_instructions[0x77] = OpCode("LD (HL),A", 0x77, 1, 8, {
			[&]() { _data.lsb = _registers.a; },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		_instructions[0xEA] = OpCode("LD (nn),A", 0xEA, 3, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _mmu.write8(BitUtils::ToUnsigned16(_data.msb, _data.lsb), _registers.a); } });
	}

	// LD specials (A, HL+, HL-)
	{
		_instructions[0xF2] = OpCode("LDH A,(C)", 0xF2, 1, 8, {
			[&]() { _data.addr = BitUtils::ToUnsigned16(0xFF, _registers.c); },
			[&]() { _registers.a = _mmu.read8(_data.addr); } });
		_instructions[0xE2] = OpCode("LDH (C),A", 0xE2, 1, 8, {
			[&]() { _data.addr = BitUtils::ToUnsigned16(0xFF, _registers.c); },
			[&]() { _mmu.write8(_data.addr, _registers.a); } });
		_instructions[0x3A] = OpCode("LD A,(HL-)", 0x3A, 1, 8, {
			[&]() { _registers.a = _mmu.read8(_registers.hl()); },
			[&]() { _registers.hl(_registers.hl() - 1); } });
		_instructions[0x32] = OpCode("LD (HL-),A", 0x32, 1, 8, {
			[&]() { _mmu.write8(_registers.hl(), _registers.a); },
			[&]() { _registers.hl(_registers.hl() - 1); } });
		_instructions[0x2A] = OpCode("LD A,(HL+)", 0x2A, 1, 8, {
			[&]() { _registers.a = _mmu.read8(_registers.hl()); },
			[&]() { _registers.hl(_registers.hl() + 1); } });
		_instructions[0x22] = OpCode("LD (HL+),A", 0x22, 1, 8, {
			[&]() { _mmu.write8(_registers.hl(), _registers.a); },
			[&]() { _registers.hl(_registers.hl() + 1); } });
		_instructions[0xF0] = OpCode("LD A,(0xFF00+n)", 0xF0, 2, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.addr = 0xFF00 + static_cast<uint16_t>(_data.lsb); },
			[&]() { _registers.a = _mmu.read8(_data.addr); } });
		_instructions[0xE0] = OpCode("LD (0xFF00+n),A", 0xE0, 2, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.addr = 0xFF00 + static_cast<uint16_t>(_data.lsb); },
			[&]() { _mmu.write8(_data.addr, _registers.a); } });
	}

	// LD rr,nn
	{
		_instructions[0x01] = OpCode("LD BC,nn", 0x01, 3, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.bc(BitUtils::ToUnsigned16(_data.msb, _data.lsb)); } });
		_instructions[0x11] = OpCode("LD DE,nn", 0x11, 3, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.de(BitUtils::ToUnsigned16(_data.msb, _data.lsb)); } });
		_instructions[0x21] = OpCode("LD HL,nn", 0x21, 3, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.hl(BitUtils::ToUnsigned16(_data.msb, _data.lsb)); } });
		_instructions[0x31] = OpCode("LD SP,nn", 0x31, 3, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.sp = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } });
	}

	// SP specials
	{
		_instructions[0x08] = OpCode("LD (nn),SP", 0x08, 3, 20, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _data.addr = BitUtils::ToUnsigned16(_data.msb, _data.lsb); },
			[&]() { _mmu.write8(_data.addr, BitUtils::GetLsb(_registers.sp)); },
			[&]() { _mmu.write8(_data.addr + 1, BitUtils::GetMsb(_registers.sp)); } });
		_instructions[0xF9] = OpCode("LD SP,HL", 0xF9, 1, 8, {
			[&]() { _registers.sp = _registers.hl(); },
			[&]() {} });
		_instructions[0xF8] = OpCode("LD HL,SP+n", 0xF8, 2, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.addr = _registers.sp + static_cast<int8_t>(_data.lsb); },
			[&]() { _registers.hl(_data.addr);
					_registers.flags.z = 0;
					_registers.flags.n = 0;
					_registers.flags.h = Registers::Flags::HalfCarry8(static_cast<uint8_t>(_registers.sp), _data.lsb);
					_registers.flags.c = Registers::Flags::Carry8(static_cast<uint8_t>(_registers.sp), _data.lsb); } });

		// PUSH rr
		_instructions[0xF5] = OpCode("PUSH AF", 0xF5, 1, 16, {
			[&]() {},
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.af())); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.af())); } });
		_instructions[0xC5] = OpCode("PUSH BC", 0xC5, 1, 16, {
			[&]() {},
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.bc())); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.bc())); } });
		_instructions[0xD5] = OpCode("PUSH DE", 0xD5, 1, 16, {
			[&]() {},
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.de())); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.de())); } });
		_instructions[0xE5] = OpCode("PUSH HL", 0xE5, 1, 16, {
			[&]() {},
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.hl())); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.hl())); } });

		// POP rr
		_instructions[0xF1] = OpCode("POP AF", 0xF1, 1, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.sp++); },
			[&]() { _data.msb = _mmu.read8(_registers.sp++); },
			[&]() { _registers.af(BitUtils::ToUnsigned16(_data.msb, _data.lsb)); } });
		_instructions[0xC1] = OpCode("POP BC", 0xC1, 1, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.sp++); },
			[&]() { _data.msb = _mmu.read8(_registers.sp++); },
			[&]() { _registers.bc(BitUtils::ToUnsigned16(_data.msb, _data.lsb)); } });
		_instructions[0xD1] = OpCode("POP DE", 0xD1, 1, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.sp++); },
			[&]() { _data.msb = _mmu.read8(_registers.sp++); },
			[&]() { _registers.de(BitUtils::ToUnsigned16(_data.msb, _data.lsb)); } });
		_instructions[0xE1] = OpCode("POP HL", 0xE1, 1, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.sp++); },
			[&]() { _data.msb = _mmu.read8(_registers.sp++); },
			[&]() { _registers.hl(BitUtils::ToUnsigned16(_data.msb, _data.lsb)); } });
	}

	// ADD
	{
		auto setAddFlags = [](Registers& registers, uint8_t a, uint8_t b) -> uint8_t {
			uint8_t r = a + b;
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = Registers::Flags::HalfCarry8(a, b);
			registers.flags.c = Registers::Flags::Carry8(a, b);
			return r; };

		_instructions[0x87] = OpCode("ADD A,A", 0x87, 1, 4, {
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _registers.a); } });
		_instructions[0x80] = OpCode("ADD A,B", 0x80, 1, 4, {
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _registers.b); } });
		_instructions[0x81] = OpCode("ADD A,C", 0x81, 1, 4, {
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _registers.c); } });
		_instructions[0x82] = OpCode("ADD A,D", 0x82, 1, 4, {
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _registers.d); } });
		_instructions[0x83] = OpCode("ADD A,E", 0x83, 1, 4, {
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _registers.e); } });
		_instructions[0x84] = OpCode("ADD A,H", 0x84, 1, 4, {
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _registers.h); } });
		_instructions[0x85] = OpCode("ADD A,L", 0x85, 1, 4, {
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _registers.l); } });
		_instructions[0x86] = OpCode("ADD A,(HL)", 0x86, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _data.lsb); } });
		_instructions[0xC6] = OpCode("ADD A,n", 0xC6, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.a = setAddFlags(_registers, _registers.a, _data.lsb); } });

		auto setAddHLFlags = [](Registers& registers, uint16_t a, uint16_t b) -> uint16_t {
			uint16_t r = a + b;
			registers.flags.n = 0;
			registers.flags.h = Registers::Flags::HalfCarry16(a, b);
			registers.flags.c = Registers::Flags::Carry16(a, b);
			return r; };

		_instructions[0x09] = OpCode("ADD HL,BC", 0x09, 1, 8, {
			[&]() { _data.u16 = _registers.bc(); },
			[&]() { _registers.hl(setAddHLFlags(_registers, _registers.hl(), _data.u16)); } });
		_instructions[0x19] = OpCode("ADD HL,DE", 0x19, 1, 8, {
			[&]() { _data.u16 = _registers.de(); },
			[&]() { _registers.hl(setAddHLFlags(_registers, _registers.hl(), _data.u16)); } });
		_instructions[0x29] = OpCode("ADD HL,HL", 0x29, 1, 8, {
			[&]() { _data.u16 = _registers.hl(); },
			[&]() { _registers.hl(setAddHLFlags(_registers, _registers.hl(), _data.u16)); } });
		_instructions[0x39] = OpCode("ADD HL,SP", 0x39, 1, 8, {
			[&]() { _data.u16 = _registers.sp; },
			[&]() { _registers.hl(setAddHLFlags(_registers, _registers.hl(), _data.u16)); } });

		_instructions[0xE8] = OpCode("ADD SP,n", 0xE8, 2, 16, {
			[&]() {},
			[&]() {},
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.flags.z = 0;
					_registers.flags.n = 0;
					_registers.flags.h = Registers::Flags::HalfCarry8(static_cast<uint8_t>(_registers.sp), _data.lsb);
					_registers.flags.c = Registers::Flags::Carry8(static_cast<uint8_t>(_registers.sp), _data.lsb);
					_registers.sp = _registers.sp + static_cast<int8_t>(_data.lsb); } });
	}

	// ADC
	{
		auto setAddCFlags = [](Registers& registers, uint8_t a, uint8_t b) -> uint8_t {
			uint8_t r = a + b + static_cast<uint8_t>(registers.flags.c);
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = Registers::Flags::HalfCarry8(a, b, static_cast<uint8_t>(registers.flags.c));
			registers.flags.c = Registers::Flags::Carry8(a, b, static_cast<uint8_t>(registers.flags.c));
			return r; };

		_instructions[0x8F] = OpCode("ADC A,A", 0x8F, 1, 4, {
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _registers.a); } });
		_instructions[0x88] = OpCode("ADC A,B", 0x88, 1, 4, {
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _registers.b); } });
		_instructions[0x89] = OpCode("ADC A,C", 0x89, 1, 4, {
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _registers.c); } });
		_instructions[0x8A] = OpCode("ADC A,D", 0x8A, 1, 4, {
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _registers.d); } });
		_instructions[0x8B] = OpCode("ADC A,E", 0x8B, 1, 4, {
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _registers.e); } });
		_instructions[0x8C] = OpCode("ADC A,H", 0x8C, 1, 4, {
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _registers.h); } });
		_instructions[0x8D] = OpCode("ADC A,L", 0x8D, 1, 4, {
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _registers.l); } });
		_instructions[0x8E] = OpCode("ADC A,(HL)", 0x8E, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _data.lsb); } });
		_instructions[0xCE] = OpCode("ADC A,n", 0xCE, 2, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.a = setAddCFlags(_registers, _registers.a, _data.lsb); } });
	}

	// SUB
	{
		auto setSubFlags = [](Registers& registers, uint8_t a, uint8_t b) -> uint8_t {
			uint8_t r = a - b;
			registers.flags.z = r == 0;
			registers.flags.n = 1;
			registers.flags.h = ((static_cast<int8_t>(a & 0xF) - static_cast<int8_t>(b & 0xF)) < 0);
			registers.flags.c = a < b;
			return r; };

		_instructions[0x97] = OpCode("SUB A,A", 0x97, 1, 4, {
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _registers.a); } });
		_instructions[0x90] = OpCode("SUB A,B", 0x90, 1, 4, {
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _registers.b); } });
		_instructions[0x91] = OpCode("SUB A,C", 0x91, 1, 4, {
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _registers.c); } });
		_instructions[0x92] = OpCode("SUB A,D", 0x92, 1, 4, {
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _registers.d); } });
		_instructions[0x93] = OpCode("SUB A,E", 0x93, 1, 4, {
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _registers.e); } });
		_instructions[0x94] = OpCode("SUB A,H", 0x94, 1, 4, {
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _registers.h); } });
		_instructions[0x95] = OpCode("SUB A,L", 0x95, 1, 4, {
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _registers.l); } });
		_instructions[0x96] = OpCode("SUB A,(HL)", 0x96, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _data.lsb); } });
		_instructions[0xD6] = OpCode("SUB A,n", 0xD6, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.a = setSubFlags(_registers, _registers.a, _data.lsb); } });
	}

	// SBC
	{
		auto setSubCFlags = [](Registers& registers, uint8_t a, uint8_t b) -> uint8_t {
			uint8_t r = a - b - static_cast<uint8_t>(registers.flags.c);
			registers.flags.z = r == 0;
			registers.flags.n = 1;
			registers.flags.h = ((static_cast<int8_t>(a & 0xF) - static_cast<int8_t>(b & 0xF) - static_cast<int8_t>(registers.flags.c)) < 0);
			registers.flags.c = a < (b + registers.flags.c);
			return r; };

		_instructions[0x9F] = OpCode("SBC A,A", 0x9F, 1, 4, {
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _registers.a); } });
		_instructions[0x98] = OpCode("SBC A,B", 0x98, 1, 4, {
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _registers.b); } });
		_instructions[0x99] = OpCode("SBC A,C", 0x99, 1, 4, {
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _registers.c); } });
		_instructions[0x9A] = OpCode("SBC A,D", 0x9A, 1, 4, {
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _registers.d); } });
		_instructions[0x9B] = OpCode("SBC A,E", 0x9B, 1, 4, {
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _registers.e); } });
		_instructions[0x9C] = OpCode("SBC A,H", 0x9C, 1, 4, {
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _registers.h); } });
		_instructions[0x9D] = OpCode("SBC A,L", 0x9D, 1, 4, {
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _registers.l); } });
		_instructions[0x9E] = OpCode("SBC A,(HL)", 0x9E, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _data.lsb); } });
		_instructions[0xDE] = OpCode("SBC A,n", 0xDE, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.a = setSubCFlags(_registers, _registers.a, _data.lsb); } });
	}

	// AND
	{
		auto setAndFlags = [](Registers& registers, uint8_t a, uint8_t b) -> uint8_t {
			uint8_t r = a & b;
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 1;
			registers.flags.c = 0;
			return r; };

		_instructions[0xA7] = OpCode("AND A,A", 0xA7, 1, 4, {
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _registers.a); } });
		_instructions[0xA0] = OpCode("AND A,B", 0xA0, 1, 4, {
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _registers.b); } });
		_instructions[0xA1] = OpCode("AND A,C", 0xA1, 1, 4, {
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _registers.c); } });
		_instructions[0xA2] = OpCode("AND A,D", 0xA2, 1, 4, {
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _registers.d); } });
		_instructions[0xA3] = OpCode("AND A,E", 0xA3, 1, 4, {
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _registers.e); } });
		_instructions[0xA4] = OpCode("AND A,H", 0xA4, 1, 4, {
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _registers.h); } });
		_instructions[0xA5] = OpCode("AND A,L", 0xA5, 1, 4, {
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _registers.l); } });
		_instructions[0xA6] = OpCode("AND A,(HL)", 0xA6, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _data.lsb); } });
		_instructions[0xE6] = OpCode("AND A,n", 0xE6, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.a = setAndFlags(_registers, _registers.a, _data.lsb); } });
	}

	// OR
	{
		auto setOrFlags = [](Registers& registers, uint8_t a, uint8_t b) -> uint8_t {
			uint8_t r = a | b;
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			registers.flags.c = 0;
			return r; };

		_instructions[0xB7] = OpCode("OR A,A", 0xB7, 1, 4, {
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _registers.a); } });
		_instructions[0xB0] = OpCode("OR A,B", 0xB0, 1, 4, {
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _registers.b); } });
		_instructions[0xB1] = OpCode("OR A,C", 0xB1, 1, 4, {
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _registers.c); } });
		_instructions[0xB2] = OpCode("OR A,D", 0xB2, 1, 4, {
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _registers.d); } });
		_instructions[0xB3] = OpCode("OR A,E", 0xB3, 1, 4, {
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _registers.e); } });
		_instructions[0xB4] = OpCode("OR A,H", 0xB4, 1, 4, {
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _registers.h); } });
		_instructions[0xB5] = OpCode("OR A,L", 0xB5, 1, 4, {
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _registers.l); } });
		_instructions[0xB6] = OpCode("OR A,(HL)", 0xB6, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _data.lsb); } });
		_instructions[0xF6] = OpCode("OR A,n", 0xF6, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.a = setOrFlags(_registers, _registers.a, _data.lsb); } });
	}

	// XOR
	{
		auto setXorFlags = [](Registers& registers, uint8_t a, uint8_t b) -> uint8_t {
			uint8_t r = a ^ b;
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			registers.flags.c = 0;
			return r; };

		_instructions[0xAF] = OpCode("XOR A,A", 0xAF, 1, 4, {
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _registers.a); } });
		_instructions[0xA8] = OpCode("XOR A,B", 0xA8, 1, 4, {
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _registers.b); } });
		_instructions[0xA9] = OpCode("XOR A,C", 0xA9, 1, 4, {
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _registers.c); } });
		_instructions[0xAA] = OpCode("XOR A,D", 0xAA, 1, 4, {
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _registers.d); } });
		_instructions[0xAB] = OpCode("XOR A,E", 0xAB, 1, 4, {
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _registers.e); } });
		_instructions[0xAC] = OpCode("XOR A,H", 0xAC, 1, 4, {
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _registers.h); } });
		_instructions[0xAD] = OpCode("XOR A,L", 0xAD, 1, 4, {
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _registers.l); } });
		_instructions[0xAE] = OpCode("XOR A,(HL)", 0xAE, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _data.lsb); } });
		_instructions[0xEE] = OpCode("XOR A,n", 0xEE, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.a = setXorFlags(_registers, _registers.a, _data.lsb); } });
	}

	// CP n
	{
		auto setCpFlags = [](Registers& registers, uint8_t a, uint8_t b) {
			uint8_t r = a - b;
			registers.flags.z = r == 0;
			registers.flags.n = 1;
			registers.flags.h = ((static_cast<int8_t>(a & 0xF) - static_cast<int8_t>(b & 0xF)) < 0);
			registers.flags.c = a < b; };

		_instructions[0xBF] = OpCode("CP A,A", 0xBF, 1, 4, {
			[&]() { setCpFlags(_registers, _registers.a, _registers.a); } });
		_instructions[0xB8] = OpCode("CP A,B", 0xB8, 1, 4, {
			[&]() { setCpFlags(_registers, _registers.a, _registers.b); } });
		_instructions[0xB9] = OpCode("CP A,C", 0xB9, 1, 4, {
			[&]() { setCpFlags(_registers, _registers.a, _registers.c); } });
		_instructions[0xBA] = OpCode("CP A,D", 0xBA, 1, 4, {
			[&]() { setCpFlags(_registers, _registers.a, _registers.d); } });
		_instructions[0xBB] = OpCode("CP A,E", 0xBB, 1, 4, {
			[&]() { setCpFlags(_registers, _registers.a, _registers.e); } });
		_instructions[0xBC] = OpCode("CP A,H", 0xBC, 1, 4, {
			[&]() { setCpFlags(_registers, _registers.a, _registers.h); } });
		_instructions[0xBD] = OpCode("CP A,L", 0xBD, 1, 4, {
			[&]() { setCpFlags(_registers, _registers.a, _registers.l); } });
		_instructions[0xBE] = OpCode("CP A,(HL)", 0xBE, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { setCpFlags(_registers, _registers.a, _data.lsb); } });
		_instructions[0xFE] = OpCode("CP A,n", 0xFE, 1, 8, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { setCpFlags(_registers, _registers.a, _data.lsb); } });
	}

	// INC r
	{
		auto setIncFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			uint8_t r = a + 1;
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = ((r & 0xF) == 0);
			return r;  };

		_instructions[0x3C] = OpCode("INC A", 0x3C, 1, 4, {
			[&]() { _registers.a = setIncFlags(_registers, _registers.a); } });
		_instructions[0x04] = OpCode("INC B", 0x04, 1, 4, {
			[&]() { _registers.b = setIncFlags(_registers, _registers.b); } });
		_instructions[0x0C] = OpCode("INC C", 0x0C, 1, 4, {
			[&]() { _registers.c = setIncFlags(_registers, _registers.c); } });
		_instructions[0x14] = OpCode("INC D", 0x14, 1, 4, {
			[&]() { _registers.d = setIncFlags(_registers, _registers.d); } });
		_instructions[0x1C] = OpCode("INC E", 0x1C, 1, 4, {
			[&]() { _registers.e = setIncFlags(_registers, _registers.e); } });
		_instructions[0x24] = OpCode("INC H", 0x24, 1, 4, {
			[&]() { _registers.h = setIncFlags(_registers, _registers.h); } });
		_instructions[0x2C] = OpCode("INC L", 0x2C, 1, 4, {
			[&]() { _registers.l = setIncFlags(_registers, _registers.l); } });
		_instructions[0x34] = OpCode("INC (HL)", 0x34, 1, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setIncFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });

		// INC nn
		_instructions[0x03] = OpCode("INC BC", 0x03, 1, 8, {
			[&]() { _data.u16 = _registers.bc() + 1; },
			[&]() { _registers.bc(_data.u16); } });
		_instructions[0x13] = OpCode("INC DE", 0x13, 1, 8, {
			[&]() { _data.u16 = _registers.de() + 1; },
			[&]() { _registers.de(_data.u16); } });
		_instructions[0x23] = OpCode("INC HL", 0x23, 1, 8, {
			[&]() { _data.u16 = _registers.hl() + 1; },
			[&]() { _registers.hl(_data.u16); } });
		_instructions[0x33] = OpCode("INC SP", 0x33, 1, 8, {
			[&]() { _data.u16 = _registers.sp + 1; },
			[&]() { _registers.sp = _data.u16; } });
	}

	// DEC r
	{
		auto setDecFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			uint8_t r = a - 1;
			registers.flags.z = r == 0;
			registers.flags.n = 1;
			registers.flags.h = ((r & 0xF) == 0xF);
			return r; };

		_instructions[0x3D] = OpCode("DEC A", 0x3D, 1, 4, {
			[&]() { _registers.a = setDecFlags(_registers, _registers.a); } });
		_instructions[0x05] = OpCode("DEC B", 0x05, 1, 4, {
			[&]() { _registers.b = setDecFlags(_registers, _registers.b); } });
		_instructions[0x0D] = OpCode("DEC C", 0x0D, 1, 4, {
			[&]() { _registers.c = setDecFlags(_registers, _registers.c); } });
		_instructions[0x15] = OpCode("DEC D", 0x15, 1, 4, {
			[&]() { _registers.d = setDecFlags(_registers, _registers.d); } });
		_instructions[0x1D] = OpCode("DEC E", 0x1D, 1, 4, {
			[&]() { _registers.e = setDecFlags(_registers, _registers.e); } });
		_instructions[0x25] = OpCode("DEC H", 0x25, 1, 4, {
			[&]() { _registers.h = setDecFlags(_registers, _registers.h); } });
		_instructions[0x2D] = OpCode("DEC L", 0x2D, 1, 4, {
			[&]() { _registers.l = setDecFlags(_registers, _registers.l); } });
		_instructions[0x35] = OpCode("DEC (HL)", 0x35, 1, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setDecFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });

		// DEC nn
		_instructions[0x0B] = OpCode("DEC BC", 0x0B, 1, 8, {
			[&]() { _data.u16 = _registers.bc() - 1; },
			[&]() { _registers.bc(_data.u16); } });
		_instructions[0x1B] = OpCode("DEC DE", 0x1B, 1, 8, {
			[&]() { _data.u16 = _registers.de() - 1; },
			[&]() { _registers.de(_data.u16); } });
		_instructions[0x2B] = OpCode("DEC HL", 0x2B, 1, 8, {
			[&]() { _data.u16 = _registers.hl() - 1; },
			[&]() { _registers.hl(_data.u16); } });
		_instructions[0x3B] = OpCode("DEC SP", 0x3B, 1, 8, {
			[&]() { _data.u16 = _registers.sp - 1; },
			[&]() { _registers.sp = _data.u16; } });
	}

	// Flags
	{
		_instructions[0x27] = OpCode("DAA", 0x27, 1, 4, {
			[&]() { uint8_t correction = 0;
					if (_registers.flags.h || (!_registers.flags.n && (_registers.a & 0x0F) > 0x09))
					{
						correction |= 0x06;
					}
					if (_registers.flags.c || (!_registers.flags.n && (_registers.a & 0xFF) > 0x99))
					{
						correction |= 0x60;
						_registers.flags.c = true;
					}
					uint8_t r = _registers.flags.n == 0 ? _registers.a + correction : _registers.a - correction;
					_registers.a = r;
					_registers.flags.h = 0;
					_registers.flags.z = r == 0; } });

		_instructions[0x2F] = OpCode("CPL", 0x2F, 1, 4, {
			[&]() { _registers.a = ~_registers.a;
					_registers.flags.n = 1;
					_registers.flags.h = 1; } });

		_instructions[0x3F] = OpCode("CCF", 0x3F, 1, 4, {
			[&]() { _registers.flags.n = 0;
					_registers.flags.h = 0;
					_registers.flags.c = !_registers.flags.c; } });

		_instructions[0x37] = OpCode("SCF", 0x37, 1, 4, {
			[&]() { _registers.flags.n = 0;
					_registers.flags.h = 0;
					_registers.flags.c = 1; } });
	}

	// Rotates & Shifts
	{
		_instructions[0x07] = OpCode("RLCA", 0x07, 1, 4, {
			[&]() { _registers.flags.c = BitUtils::GetBit(_registers.a, 7);
					_registers.a = (_registers.a << 1) | static_cast<uint8_t>(_registers.flags.c);
					_registers.flags.z = 0;
					_registers.flags.n = 0;
					_registers.flags.h = 0; } });

		_instructions[0x17] = OpCode("RLA", 0x17, 1, 4, {
			[&]() { bool c = BitUtils::GetBit(_registers.a, 7);
					_registers.a = (_registers.a << 1) | static_cast<uint8_t>(_registers.flags.c);
					_registers.flags.z = 0;
					_registers.flags.n = 0;
					_registers.flags.h = 0;
					_registers.flags.c = c; } });

		_instructions[0x0F] = OpCode("RRCA", 0x0F, 1, 4, {
			[&]() { _registers.flags.c = BitUtils::GetBit(_registers.a, 0);
					_registers.a = (_registers.a >> 1) | (_registers.flags.c << 7);
					_registers.flags.z = 0;
					_registers.flags.n = 0;
					_registers.flags.h = 0; } });

		_instructions[0x1F] = OpCode("RRA", 0x1F, 1, 4, {
			[&]() { bool c = BitUtils::GetBit(_registers.a, 0);
					_registers.a = (_registers.a >> 1) | (static_cast<uint8_t>(_registers.flags.c) << 7);
					_registers.flags.z = 0;
					_registers.flags.n = 0;
					_registers.flags.h = 0;
					_registers.flags.c = c; } });
	}

	// Miscellaneous
	{
		_instructions[0x00] = OpCode("NOP", 0x00, 1, 4, {
				[&]() {} });
		_instructions[0x76] = OpCode("HALT", 0x76, 1, 4, {
				[&]() { if (_interrupts.isHaltBug()) _haltBug = true; else _halt = true; } });
		_instructions[0x10] = OpCode("STOP", 0x10, 1, 4, {
				[&]() { _registers.pc++; } });
		_instructions[0xF3] = OpCode("DI", 0xF3, 1, 4, {
				[&]() { _interrupts.setIme(false);
						_interruptEnableRequest = false; } });
		_instructions[0xFB] = OpCode("EI", 0xFB, 1, 4, {
				[&]() { _interruptEnableRequest = true; } });
	}

	// JUMPS
	{
		_instructions[0xC3] = OpCode("JP nn", 0xC3, 3, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _data.addr = BitUtils::ToUnsigned16(_data.msb, _data.lsb); },
			[&]() { _registers.pc = _data.addr; } });

		_instructions[0xC2] = OpCode("JP NZ,nn", 0xC2, 3, 12, { // 16 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.z == 0) {
			_data.overrideCycles += 4;
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xCA] = OpCode("JP Z,nn", 0xCA, 3, 12, { // 16 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.z == 1) {
			_data.overrideCycles += 4;
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xD2] = OpCode("JP NC,nn", 0xD2, 3, 12, { // 16 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.c == 0) {
			_data.overrideCycles += 4;
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xDA] = OpCode("JP C,nn", 0xDA, 3, 12, { // 16 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.c == 1) {
			_data.overrideCycles += 4;
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xE9] = OpCode("JP (HL)", 0xE9, 1, 4, {
			[&]() { _registers.pc = _registers.hl(); } });

		_instructions[0x18] = OpCode("JR n", 0x18, 2, 12, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.pc += static_cast<int8_t>(_data.lsb); } });

		_instructions[0x20] = OpCode("JR NZ,n", 0x20, 2, 8, { // 12 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.z == 0) {
			_data.overrideCycles += 4;
			_registers.pc += static_cast<int8_t>(_data.lsb); } } });

		_instructions[0x28] = OpCode("JR Z,n", 0x28, 2, 8, { // 12 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.z == 1) {
			_data.overrideCycles += 4;
			_registers.pc += static_cast<int8_t>(_data.lsb); } } });

		_instructions[0x30] = OpCode("JR NC,n", 0x30, 2, 8, { // 12 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.c == 0) {
			_data.overrideCycles += 4;
			_registers.pc += static_cast<int8_t>(_data.lsb); } } });

		_instructions[0x38] = OpCode("JR C,n", 0x38, 2, 8, { // 12 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.c == 1) {
			_data.overrideCycles += 4;
			_registers.pc += static_cast<int8_t>(_data.lsb); } } });
	}

	// CALL
	{
		_instructions[0xCD] = OpCode("CALL nn", 0xCD, 3, 24, {
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } });

		_instructions[0xC4] = OpCode("CALL NZ,nn", 0xC4, 3, 12, { // 24 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.z == 0) {
			_data.overrideCycles += 12;
			_registers.sp--;
			_mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc));
			_mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc));
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xCC] = OpCode("CALL Z,nn", 0xCC, 3, 12, { // 24 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.z == 1) {
			_data.overrideCycles += 12;
			_registers.sp--;
			_mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc));
			_mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc));
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xD4] = OpCode("CALL NC,nn", 0xD4, 3, 12, { // 24 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.c == 0) {
			_data.overrideCycles += 12;
			_registers.sp--;
			_mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc));
			_mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc));
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xDC] = OpCode("CALL C,nn", 0xDC, 3, 12, { // 24 with branch
			[&]() { _data.lsb = _mmu.read8(_registers.pc++); },
			[&]() { _data.msb = _mmu.read8(_registers.pc++); },
			[&]() { if (_registers.flags.c == 1) {
			_data.overrideCycles += 12;
			_registers.sp--;
			_mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc));
			_mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc));
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });
	}

	// RST
	{
		_instructions[0xC7] = OpCode("RST 00", 0xC7, 1, 16, {
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, 0x00); } });
		_instructions[0xCF] = OpCode("RST 08", 0xCF, 1, 16, {
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, 0x08); } });
		_instructions[0xD7] = OpCode("RST 10", 0xD7, 1, 16, {
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, 0x10); } });
		_instructions[0xDF] = OpCode("RST 18", 0xDF, 1, 16, {
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, 0x18); } });
		_instructions[0xE7] = OpCode("RST 20", 0xE7, 1, 16, {
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, 0x20); } });
		_instructions[0xEF] = OpCode("RST 28", 0xEF, 1, 16, {
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, 0x28); } });
		_instructions[0xF7] = OpCode("RST 30", 0xF7, 1, 16, {
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, 0x30); } });
		_instructions[0xFF] = OpCode("RST 38", 0xFF, 1, 16, {
			[&]() { _registers.sp--; },
			[&]() { _mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
			[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, 0x38); } });
	}

	// RET
	{
		_instructions[0xC9] = OpCode("RET nn", 0xC9, 1, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.sp++); },
			[&]() { _data.msb = _mmu.read8(_registers.sp++); },
			[&]() { _data.addr = BitUtils::ToUnsigned16(_data.msb, _data.lsb); },
			[&]() { _registers.pc = _data.addr; } });

		_instructions[0xC0] = OpCode("RET NZ", 0xC0, 1, 8, { // 20 with branch
			[&]() {},
			[&]() { if (_registers.flags.z == 0) {
			_data.overrideCycles += 12;
			_data.lsb = _mmu.read8(_registers.sp++);
			_data.msb = _mmu.read8(_registers.sp++);
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xC8] = OpCode("RET Z", 0xC8, 1, 8, { // 20 with branch
			[&]() {},
			[&]() { if (_registers.flags.z == 1) {
			_data.overrideCycles += 12;
			_data.lsb = _mmu.read8(_registers.sp++);
			_data.msb = _mmu.read8(_registers.sp++);
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xD0] = OpCode("RET NC", 0xD0, 1, 8, { // 20 with branch
			[&]() {},
			[&]() { if (_registers.flags.c == 0) {
			_data.overrideCycles += 12;
			_data.lsb = _mmu.read8(_registers.sp++);
			_data.msb = _mmu.read8(_registers.sp++);
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xD8] = OpCode("RET C", 0xD8, 1, 8, { // 20 with branch
			[&]() {},
			[&]() { if (_registers.flags.c == 1) {
			_data.overrideCycles += 12;
			_data.lsb = _mmu.read8(_registers.sp++);
			_data.msb = _mmu.read8(_registers.sp++);
			_registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); } } });

		_instructions[0xD9] = OpCode("RETI", 0xD9, 1, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.sp++); },
			[&]() { _data.msb = _mmu.read8(_registers.sp++); },
			[&]() { _registers.pc = BitUtils::ToUnsigned16(_data.msb, _data.lsb); },
			[&]() { _interrupts.setIme(true); } });
	}

	// PREFIX CB
	{
		_instructions[0xCB] = OpCode("PREFIX CB", 0xCB, 1, 4, {
			[&]() {} });
	}

	// SWAP r
	{
		auto setSwapFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			uint8_t r = BitUtils::Swap(a);
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			registers.flags.c = 0;
			return r; };

		_instructionsCB[0x37] = OpCode("SWAP A", 0x37, 1, 8, {
			[&]() { _registers.a = setSwapFlags(_registers, _registers.a); } });
		_instructionsCB[0x30] = OpCode("SWAP B", 0x30, 1, 8, {
			[&]() { _registers.b = setSwapFlags(_registers, _registers.b); } });
		_instructionsCB[0x31] = OpCode("SWAP C", 0x31, 1, 8, {
			[&]() { _registers.c = setSwapFlags(_registers, _registers.c); } });
		_instructionsCB[0x32] = OpCode("SWAP D", 0x32, 1, 8, {
			[&]() { _registers.d = setSwapFlags(_registers, _registers.d); } });
		_instructionsCB[0x33] = OpCode("SWAP E", 0x33, 1, 8, {
			[&]() { _registers.e = setSwapFlags(_registers, _registers.e); } });
		_instructionsCB[0x34] = OpCode("SWAP H", 0x34, 1, 8, {
			[&]() { _registers.h = setSwapFlags(_registers, _registers.h); } });
		_instructionsCB[0x35] = OpCode("SWAP L", 0x35, 1, 8, {
			[&]() { _registers.l = setSwapFlags(_registers, _registers.l); } });
		_instructionsCB[0x36] = OpCode("SWAP (HL)", 0x36, 1, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setSwapFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// RLC r
	{
		auto setRlcFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			registers.flags.c = BitUtils::GetBit(a, 7);
			uint8_t r = (a << 1) | static_cast<uint8_t>(registers.flags.c);
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			return r; };

		_instructionsCB[0x07] = OpCode("RLC A", 0x07, 2, 8, {
			[&]() { _registers.a = setRlcFlags(_registers, _registers.a); } });
		_instructionsCB[0x00] = OpCode("RLC B", 0x00, 2, 8, {
			[&]() { _registers.b = setRlcFlags(_registers, _registers.b); } });
		_instructionsCB[0x01] = OpCode("RLC C", 0x01, 2, 8, {
			[&]() { _registers.c = setRlcFlags(_registers, _registers.c); } });
		_instructionsCB[0x02] = OpCode("RLC D", 0x02, 2, 8, {
			[&]() { _registers.d = setRlcFlags(_registers, _registers.d); } });
		_instructionsCB[0x03] = OpCode("RLC E", 0x03, 2, 8, {
			[&]() { _registers.e = setRlcFlags(_registers, _registers.e); } });
		_instructionsCB[0x04] = OpCode("RLC H", 0x04, 2, 8, {
			[&]() { _registers.h = setRlcFlags(_registers, _registers.h); } });
		_instructionsCB[0x05] = OpCode("RLC L", 0x05, 2, 8, {
			[&]() { _registers.l = setRlcFlags(_registers, _registers.l); } });
		_instructionsCB[0x06] = OpCode("RLC (HL)", 0x06, 2, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setRlcFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// RL r
	{
		auto setRlFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			bool c = BitUtils::GetBit(a, 7);
			uint8_t r = (a << 1) | static_cast<uint8_t>(registers.flags.c);
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			registers.flags.c = c;
			return r; };

		_instructionsCB[0x17] = OpCode("RL A", 0x17, 2, 8, {
			[&]() { _registers.a = setRlFlags(_registers, _registers.a); } });
		_instructionsCB[0x10] = OpCode("RL B", 0x10, 2, 8, {
			[&]() { _registers.b = setRlFlags(_registers, _registers.b); } });
		_instructionsCB[0x11] = OpCode("RL C", 0x11, 2, 8, {
			[&]() { _registers.c = setRlFlags(_registers, _registers.c); } });
		_instructionsCB[0x12] = OpCode("RL D", 0x12, 2, 8, {
			[&]() { _registers.d = setRlFlags(_registers, _registers.d); } });
		_instructionsCB[0x13] = OpCode("RL E", 0x13, 2, 8, {
			[&]() { _registers.e = setRlFlags(_registers, _registers.e); } });
		_instructionsCB[0x14] = OpCode("RL H", 0x14, 2, 8, {
			[&]() { _registers.h = setRlFlags(_registers, _registers.h); } });
		_instructionsCB[0x15] = OpCode("RL L", 0x15, 2, 8, {
			[&]() { _registers.l = setRlFlags(_registers, _registers.l); } });
		_instructionsCB[0x16] = OpCode("RL (HL)", 0x16, 2, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setRlFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// RRC r
	{
		auto setRrcFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			registers.flags.c = BitUtils::GetBit(a, 0);
			uint8_t r = (a >> 1) | (static_cast<uint8_t>(registers.flags.c) << 7);
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			return r; };

		_instructionsCB[0x0F] = OpCode("RRC A", 0x0F, 2, 8, {
			[&]() { _registers.a = setRrcFlags(_registers, _registers.a); } });
		_instructionsCB[0x08] = OpCode("RRC B", 0x08, 2, 8, {
			[&]() { _registers.b = setRrcFlags(_registers, _registers.b); } });
		_instructionsCB[0x09] = OpCode("RRC C", 0x09, 2, 8, {
			[&]() { _registers.c = setRrcFlags(_registers, _registers.c); } });
		_instructionsCB[0x0A] = OpCode("RRC D", 0x0A, 2, 8, {
			[&]() { _registers.d = setRrcFlags(_registers, _registers.d); } });
		_instructionsCB[0x0B] = OpCode("RRC E", 0x0B, 2, 8, {
			[&]() { _registers.e = setRrcFlags(_registers, _registers.e); } });
		_instructionsCB[0x0C] = OpCode("RRC H", 0x0C, 2, 8, {
			[&]() { _registers.h = setRrcFlags(_registers, _registers.h); } });
		_instructionsCB[0x0D] = OpCode("RRC L", 0x0D, 2, 8, {
			[&]() { _registers.l = setRrcFlags(_registers, _registers.l); } });
		_instructionsCB[0x0E] = OpCode("RRC (HL)", 0x0E, 2, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setRrcFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// RR r
	{
		auto setRrFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			bool c = BitUtils::GetBit(a, 0);
			uint8_t r = (a >> 1) | (static_cast<uint8_t>(registers.flags.c) << 7);
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			registers.flags.c = c;
			return r; };

		_instructionsCB[0x1F] = OpCode("RR A", 0x1F, 2, 8, {
			[&]() { _registers.a = setRrFlags(_registers, _registers.a); } });
		_instructionsCB[0x18] = OpCode("RR B", 0x18, 2, 8, {
			[&]() { _registers.b = setRrFlags(_registers, _registers.b); } });
		_instructionsCB[0x19] = OpCode("RR C", 0x19, 2, 8, {
			[&]() { _registers.c = setRrFlags(_registers, _registers.c); } });
		_instructionsCB[0x1A] = OpCode("RR D", 0x1A, 2, 8, {
			[&]() { _registers.d = setRrFlags(_registers, _registers.d); } });
		_instructionsCB[0x1B] = OpCode("RR E", 0x1B, 2, 8, {
			[&]() { _registers.e = setRrFlags(_registers, _registers.e); } });
		_instructionsCB[0x1C] = OpCode("RR H", 0x1C, 2, 8, {
			[&]() { _registers.h = setRrFlags(_registers, _registers.h); } });
		_instructionsCB[0x1D] = OpCode("RR L", 0x1D, 2, 8, {
			[&]() { _registers.l = setRrFlags(_registers, _registers.l); } });
		_instructionsCB[0x1E] = OpCode("RR (HL)", 0x1E, 2, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setRrFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// SLA r
	{
		auto setSlaFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			uint8_t r = a << 1;
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			registers.flags.c = BitUtils::GetBit(a, 7);
			return r; };

		_instructionsCB[0x27] = OpCode("SLA A", 0x27, 2, 8, {
			[&]() { _registers.a = setSlaFlags(_registers, _registers.a); } });
		_instructionsCB[0x20] = OpCode("SLA B", 0x20, 2, 8, {
			[&]() { _registers.b = setSlaFlags(_registers, _registers.b); } });
		_instructionsCB[0x21] = OpCode("SLA C", 0x21, 2, 8, {
			[&]() { _registers.c = setSlaFlags(_registers, _registers.c); } });
		_instructionsCB[0x22] = OpCode("SLA D", 0x22, 2, 8, {
			[&]() { _registers.d = setSlaFlags(_registers, _registers.d); } });
		_instructionsCB[0x23] = OpCode("SLA E", 0x23, 2, 8, {
			[&]() { _registers.e = setSlaFlags(_registers, _registers.e); } });
		_instructionsCB[0x24] = OpCode("SLA H", 0x24, 2, 8, {
			[&]() { _registers.h = setSlaFlags(_registers, _registers.h); } });
		_instructionsCB[0x25] = OpCode("SLA L", 0x25, 2, 8, {
			[&]() { _registers.l = setSlaFlags(_registers, _registers.l); } });
		_instructionsCB[0x26] = OpCode("SLA (HL)", 0x26, 2, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setSlaFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// SRA r
	{
		auto setSraFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			uint8_t r = (a >> 1) | (a & 0x80);
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			registers.flags.c = BitUtils::GetBit(a, 0);
			return r; };

		_instructionsCB[0x2F] = OpCode("SRA A", 0x2F, 2, 8, {
			[&]() { _registers.a = setSraFlags(_registers, _registers.a); } });
		_instructionsCB[0x28] = OpCode("SRA B", 0x28, 2, 8, {
			[&]() { _registers.b = setSraFlags(_registers, _registers.b); } });
		_instructionsCB[0x29] = OpCode("SRA C", 0x29, 2, 8, {
			[&]() { _registers.c = setSraFlags(_registers, _registers.c); } });
		_instructionsCB[0x2A] = OpCode("SRA D", 0x2A, 2, 8, {
			[&]() { _registers.d = setSraFlags(_registers, _registers.d); } });
		_instructionsCB[0x2B] = OpCode("SRA E", 0x2B, 2, 8, {
			[&]() { _registers.e = setSraFlags(_registers, _registers.e); } });
		_instructionsCB[0x2C] = OpCode("SRA H", 0x2C, 2, 8, {
			[&]() { _registers.h = setSraFlags(_registers, _registers.h); } });
		_instructionsCB[0x2D] = OpCode("SRA L", 0x2D, 2, 8, {
			[&]() { _registers.l = setSraFlags(_registers, _registers.l); } });
		_instructionsCB[0x2E] = OpCode("SRA (HL)", 0x2E, 2, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setSraFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// SRL r
	{
		auto setSrlFlags = [](Registers& registers, uint8_t a) -> uint8_t {
			uint8_t r = a >> 1;
			registers.flags.z = r == 0;
			registers.flags.n = 0;
			registers.flags.h = 0;
			registers.flags.c = BitUtils::GetBit(a, 0);
			return r; };

		_instructionsCB[0x3F] = OpCode("SRL A", 0x3F, 2, 8, {
			[&]() { _registers.a = setSrlFlags(_registers, _registers.a); } });
		_instructionsCB[0x38] = OpCode("SRL B", 0x38, 2, 8, {
			[&]() { _registers.b = setSrlFlags(_registers, _registers.b); } });
		_instructionsCB[0x39] = OpCode("SRL C", 0x39, 2, 8, {
			[&]() { _registers.c = setSrlFlags(_registers, _registers.c); } });
		_instructionsCB[0x3A] = OpCode("SRL D", 0x3A, 2, 8, {
			[&]() { _registers.d = setSrlFlags(_registers, _registers.d); } });
		_instructionsCB[0x3B] = OpCode("SRL E", 0x3B, 2, 8, {
			[&]() { _registers.e = setSrlFlags(_registers, _registers.e); } });
		_instructionsCB[0x3C] = OpCode("SRL H", 0x3C, 2, 8, {
			[&]() { _registers.h = setSrlFlags(_registers, _registers.h); } });
		_instructionsCB[0x3D] = OpCode("SRL L", 0x3D, 2, 8, {
			[&]() { _registers.l = setSrlFlags(_registers, _registers.l); } });
		_instructionsCB[0x3E] = OpCode("SRL (HL)", 0x3E, 2, 16, {
			[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
			[&]() { _data.lsb = setSrlFlags(_registers, _data.lsb); },
			[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
	}

	// BIT b,r
	{
		auto setBitFlags = [](Registers& registers, uint8_t a, uint8_t pos) {
			registers.flags.z = !BitUtils::GetBit(a, pos);
			registers.flags.n = 0;
			registers.flags.h = 1; };

		for (uint8_t i = 0; i < 8; i++)
		{
			_instructionsCB[0x47 + (i * 8)] = OpCode(("BIT " + std::to_string(i) + ",A").c_str(), 0x47 + (i * 8), 2, 8, {
				[&, i]() { setBitFlags(_registers, _registers.a, i); } });
			_instructionsCB[0x40 + (i * 8)] = OpCode(("BIT " + std::to_string(i) + ",B").c_str(), 0x40 + (i * 8), 2, 8, {
				[&, i]() { setBitFlags(_registers, _registers.b, i); } });
			_instructionsCB[0x41 + (i * 8)] = OpCode(("BIT " + std::to_string(i) + ",C").c_str(), 0x41 + (i * 8), 2, 8, {
				[&, i]() { setBitFlags(_registers, _registers.c, i); } });
			_instructionsCB[0x42 + (i * 8)] = OpCode(("BIT " + std::to_string(i) + ",D").c_str(), 0x42 + (i * 8), 2, 8, {
				[&, i]() { setBitFlags(_registers, _registers.d, i); } });
			_instructionsCB[0x43 + (i * 8)] = OpCode(("BIT " + std::to_string(i) + ",E").c_str(), 0x43 + (i * 8), 2, 8, {
				[&, i]() { setBitFlags(_registers, _registers.e, i); } });
			_instructionsCB[0x44 + (i * 8)] = OpCode(("BIT " + std::to_string(i) + ",H").c_str(), 0x44 + (i * 8), 2, 8, {
				[&, i]() { setBitFlags(_registers, _registers.h, i); } });
			_instructionsCB[0x45 + (i * 8)] = OpCode(("BIT " + std::to_string(i) + ",L").c_str(), 0x45 + (i * 8), 2, 8, {
				[&, i]() { setBitFlags(_registers, _registers.l, i); } });
			_instructionsCB[0x46 + (i * 8)] = OpCode(("BIT " + std::to_string(i) + ",(HL)").c_str(), 0x46 + (i * 8), 2, 12, {
				[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
				[&, i]() { setBitFlags(_registers, _data.lsb, i); },
				[&]() {} });
		}
	}

	// SET b,r
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			_instructionsCB[0xC7 + (i * 8)] = OpCode(("SET " + std::to_string(i) + ",A").c_str(), 0xC7 + (i * 8), 2, 8, {
				[&, i]() { _registers.a = BitUtils::SetBit(_registers.a, i); } });
			_instructionsCB[0xC0 + (i * 8)] = OpCode(("SET " + std::to_string(i) + ",B").c_str(), 0xC0 + (i * 8), 2, 8, {
				[&, i]() { _registers.b = BitUtils::SetBit(_registers.b, i); } });
			_instructionsCB[0xC1 + (i * 8)] = OpCode(("SET " + std::to_string(i) + ",C").c_str(), 0xC1 + (i * 8), 2, 8, {
				[&, i]() { _registers.c = BitUtils::SetBit(_registers.c, i); } });
			_instructionsCB[0xC2 + (i * 8)] = OpCode(("SET " + std::to_string(i) + ",D").c_str(), 0xC2 + (i * 8), 2, 8, {
				[&, i]() { _registers.d = BitUtils::SetBit(_registers.d, i); } });
			_instructionsCB[0xC3 + (i * 8)] = OpCode(("SET " + std::to_string(i) + ",E").c_str(), 0xC3 + (i * 8), 2, 8, {
				[&, i]() { _registers.e = BitUtils::SetBit(_registers.e, i); } });
			_instructionsCB[0xC4 + (i * 8)] = OpCode(("SET " + std::to_string(i) + ",H").c_str(), 0xC4 + (i * 8), 2, 8, {
				[&, i]() { _registers.h = BitUtils::SetBit(_registers.h, i); } });
			_instructionsCB[0xC5 + (i * 8)] = OpCode(("SET " + std::to_string(i) + ",L").c_str(), 0xC5 + (i * 8), 2, 8, {
				[&, i]() { _registers.l = BitUtils::SetBit(_registers.l, i); } });
			_instructionsCB[0xC6 + (i * 8)] = OpCode(("SET " + std::to_string(i) + ",(HL)").c_str(), 0xC6 + (i * 8), 2, 16, {
				[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
				[&, i]() { _data.lsb = BitUtils::SetBit(_data.lsb, i); },
				[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		}
	}

	// RES b,r
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			_instructionsCB[0x87 + (i * 8)] = OpCode(("RES " + std::to_string(i) + ",A").c_str(), 0x87 + (i * 8), 2, 8, {
				[&, i]() { _registers.a = BitUtils::UnsetBit(_registers.a, i); } });
			_instructionsCB[0x80 + (i * 8)] = OpCode(("RES " + std::to_string(i) + ",B").c_str(), 0x80 + (i * 8), 2, 8, {
				[&, i]() { _registers.b = BitUtils::UnsetBit(_registers.b, i); } });
			_instructionsCB[0x81 + (i * 8)] = OpCode(("RES " + std::to_string(i) + ",C").c_str(), 0x81 + (i * 8), 2, 8, {
				[&, i]() { _registers.c = BitUtils::UnsetBit(_registers.c, i); } });
			_instructionsCB[0x82 + (i * 8)] = OpCode(("RES " + std::to_string(i) + ",D").c_str(), 0x82 + (i * 8), 2, 8, {
				[&, i]() { _registers.d = BitUtils::UnsetBit(_registers.d, i); } });
			_instructionsCB[0x83 + (i * 8)] = OpCode(("RES " + std::to_string(i) + ",E").c_str(), 0x83 + (i * 8), 2, 8, {
				[&, i]() { _registers.e = BitUtils::UnsetBit(_registers.e, i); } });
			_instructionsCB[0x84 + (i * 8)] = OpCode(("RES " + std::to_string(i) + ",H").c_str(), 0x84 + (i * 8), 2, 8, {
				[&, i]() { _registers.h = BitUtils::UnsetBit(_registers.h, i); } });
			_instructionsCB[0x85 + (i * 8)] = OpCode(("RES " + std::to_string(i) + ",L").c_str(), 0x85 + (i * 8), 2, 8, {
				[&, i]() { _registers.l = BitUtils::UnsetBit(_registers.l, i); } });
			_instructionsCB[0x86 + (i * 8)] = OpCode(("RES " + std::to_string(i) + ",(HL)").c_str(), 0x86 + (i * 8), 2, 16, {
				[&]() { _data.lsb = _mmu.read8(_registers.hl()); },
				[&, i]() { _data.lsb = BitUtils::UnsetBit(_data.lsb, i); },
				[&]() { _mmu.write8(_registers.hl(), _data.lsb); } });
		}
	}
}