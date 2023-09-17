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

	logfile.open("log.txt");
	cmpfile.open("LogsCmp\\Blargg11LYStubbed.txt", std::ifstream::in);
}

CPU::~CPU()
{
	logfile.close();
	cmpfile.close();
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

void CPU::checkWithLogs()
{
	return;
	if (_registers.pc >= 0x100)
	{
		std::stringstream ss;
		ss << _registers.ToString() << " (" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)_mmu.read8(_registers.pc) << " " << std::setfill('0') << std::setw(2) << (int)_mmu.read8(_registers.pc + 1) << " " << std::setfill('0') << std::setw(2) << (int)_mmu.read8(_registers.pc + 2) << " " << std::setfill('0') << std::setw(2) << (int)_mmu.read8(_registers.pc + 3) << ")";
		//std::cout << std::dec << i << " " << ss.str() << std::endl;
		//logfile << ss.str() << std::endl;

		std::string line;
		std::getline(cmpfile, line);
		if (line == "")
		{
			std::cout << "File empty" << std::endl;
		}
		if (line != ss.str())
		{
			std::cout << "Diff at line : " << std::dec << count << std::endl;
			std::cout << ss.str() << std::endl;
			std::cout << line << std::endl;
			//_mmu.dump();
		}
		count++;
	}
}

size_t CPU::update()
{
	Interrupts::Handler* interruptHandler = _interrupts.handleInterrupts();
	if (interruptHandler != nullptr)
	{
		_halt = false;
		std::cout << "stop halt: interrupt service routine" << std::endl;
		interruptServiceRoutine(interruptHandler->addr);
		_interrupts.clearInterrupt(interruptHandler->type);
		_interrupts.setIme(false);
		std::cout << "ime CPU false " << std::endl;
	}

	if (_halt)
	{
		if (_interrupts.hasPendingInterrupts())
		{
			std::cout << "stop halt: pending interrupt" << std::endl;
			_halt = false;
		}
		else
		{
			checkWithLogs();
			return 1;
		}
	}

	if (_interruptEnableRequest)
	{
		std::cout << "ime true after request" << std::endl;
		_interrupts.setIme(true);
		_interruptEnableRequest = false;
	}

	checkWithLogs();

	uint8_t opCode = fetchInstruction();
	bool isPrefixCB = opCode == 0xCB;
	if (isPrefixCB)
	{
		opCode = fetchInstruction();
	}
	OpCode & instruction = disassembleInstruction(opCode, isPrefixCB);

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
	}
	
	_registers.pc &= 0xFFFF;
	return instruction.tcycles / 4;
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