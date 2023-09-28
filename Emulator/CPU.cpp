#include "CPU.hpp"
#include "HardwareRegisters.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

CPU::CPU(MMU& mmu, Interrupts& interrupts) :
	_mmu(mmu),
	_interrupts(interrupts),
	_halt(false),
	_haltBug(false),
	_interruptEnableRequest(false)
{
	_registers.a = 0x01;
	_registers.b = 0;
	_registers.c = 0x13;
	_registers.d = 0;
	_registers.e = 0xD8;
	_registers.f = 0;
	_registers.h = 0x01;
	_registers.l = 0x4D;
	_registers.pc = CPU::START_PC_ADDR;
	_registers.sp = 0xFFFE;
	_registers.flags.z = 1;
	_registers.flags.n = 0;
	_registers.flags.h = 1;
	_registers.flags.c = 1;
	_registers.af();
	initInstructions();
}


uint8_t CPU::fetchInstruction()
{
	return _mmu.read8(_registers.pc++);
}

CPU::OpCode & CPU::disassembleInstruction(uint8_t opCode, bool isPrefixCB)
{
	if (isPrefixCB)
	{
		if (_instructionsCB.count(opCode) == 0)
		{
			std::cout << "OpCode prefix CB: " << BitUtils::ToString(opCode) << " not implemented." << std::endl;
		}
		return _instructionsCB[opCode];
	}

	if (_instructions.count(opCode) == 0)
	{
		std::cout << "OpCode: " << BitUtils::ToString(opCode) << " not implemented." << std::endl;
	}
	return _instructions[opCode];
}

size_t CPU::update()
{
	_data.overrideCycles = 0;
	Interrupts::Handler* interruptHandler = _interrupts.handleInterrupts();
	if (interruptHandler != nullptr)
	{
		if (_interrupts.getIme())
		{
			_halt = false;
			//std::cout << "stop halt: interrupt service routine" << std::endl;
			interruptServiceRoutine(interruptHandler->addr);
			_data.overrideCycles += 20;
			_interrupts.clearInterrupt(interruptHandler->type);
			_interrupts.setIme(false);
			std::cout << "tick in halt: " << count << std::endl;
			count = 0;
		}
	}

	if (_halt)
	{
		if (_interrupts.hasPendingInterrupts())
		{
			//std::cout << "stop halt: pending interrupt" << std::endl;
			_halt = false;
			std::cout << "tick in halt: " << count << std::endl;
			count = 0;
		}
		else
		{
			count++;
			return 4;
		}
	}

	if (_interruptEnableRequest)
	{
		_interrupts.setIme(true);
		_interruptEnableRequest = false;
	}

	uint8_t opCode = fetchInstruction();
	bool isPrefixCB = opCode == 0xCB;
	if (isPrefixCB)
	{
		opCode = fetchInstruction();
	}
	OpCode & instruction = disassembleInstruction(opCode, isPrefixCB);

	if (_haltBug)
	{
		_registers.pc--;
		_haltBug = false;
	}

	for (size_t i = 0; i < instruction.steps.size(); i++)
	{
		// Excute instruction step
		instruction.steps[i]();
	}

	// TODO Serial
	static std::string s = "";
	if (_mmu.read8(HardwareRegisters::SC_ADDR) == 0x81)
	{
		s += _mmu.read8(HardwareRegisters::SB_ADDR);
		char c =_mmu.read8(HardwareRegisters::SB_ADDR);
		//std::cout << s << std::endl;
		_mmu.write8(HardwareRegisters::SC_ADDR, 0x01);
		//_interrupts.raiseInterrupt(Interrupts::Type::Serial);
		//_mmu.dump();
	}
	
	_registers.pc &= 0xFFFF;
	return instruction.tcycles + _data.overrideCycles;
}

void CPU::interruptServiceRoutine(uint16_t addr)
{
	// TODO: regular call, qe coul turn that into an OpCode
	//// 2 empty M-cycles
	//_registers.sp -= 2;
	//// 2 more M-cycles
	//_mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc));
	//_mmu.write8(_registers.sp + 1, BitUtils::GetMsb(_registers.pc));
	//// 1 more M-cycle
	//_registers.pc = addr;

	// 2 empty M-cycles
	_registers.sp--;
	// 2 more M-cycles
	_mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc));
	_mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc));
	// 1 more M-cycle
	_registers.pc = addr;
}