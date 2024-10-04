#include "CPU.hpp"
#include "HardwareRegisters.hpp"
#include "MMU.hpp"
#include "Interrupts.hpp"
#include <iostream>
#include <string>

CPU::CPU(MMU& mmu, Interrupts& interrupts) :
	_mmu(mmu),
	_interrupts(interrupts),
	_state(State::Fetch),
	_halt(false),
	_haltBug(false),
	_interruptEnableRequest(false),
	_interruptEnableRequestValue(false),
	_opCodeByte(0),
	_currentOpCode(nullptr),
	_currentInstruction(0)
{
	_registers.a = 0;
	_registers.b = 0;
	_registers.c = 0;
	_registers.d = 0;
	_registers.e = 0;
	_registers.f = 0;
	_registers.h = 0;
	_registers.l = 0;
	_registers.pc = 0;
	_registers.sp = 0xFFFE;
	_registers.flags.z = 0;
	_registers.flags.n = 0;
	_registers.flags.h = 0;
	_registers.flags.c = 0;
	_registers.af();
}

void CPU::initialize()
{
	initInstructions();

	_isr = OpCode("ISR", 0x00, 1, 20, {
		[&]() {},
		[&]() {},
		[&]() { _registers.sp--;
				_mmu.write8(_registers.sp--, BitUtils::GetMsb(_registers.pc)); },
		[&]() { _mmu.write8(_registers.sp, BitUtils::GetLsb(_registers.pc)); },
		[&]() { _registers.pc = BitUtils::ToUnsigned16(0x00, _isr.value); } });
}

void CPU::tick()
{
	_ticks = (_ticks + 1) % 4;

	if (_ticks != 0)
	{
		return;
	}

	if (_state == State::Fetch || _state == State::FetchExt || _state == State::Halt)
	{
		if (_interrupts.getIme() && _interrupts.hasPendingInterrupts())
		{
			_state = State::Interrupt;
		}
	}

	if (_interruptEnableRequest)
	{
		_interruptEnableRequest = false;
		_interrupts.setIme(_interruptEnableRequestValue);
	}

	static std::string s = "";
	if (_mmu.read8(HardwareRegisters::SC_ADDR) == 0x81)
	{
		s += _mmu.read8(HardwareRegisters::SB_ADDR);
		char c = _mmu.read8(HardwareRegisters::SB_ADDR);
		//std::cout << c;
		_mmu.write8(HardwareRegisters::SC_ADDR, 0x01);
	}

	while (true)
	{
		switch (_state)
		{
		case State::Fetch:
		{
			_opCodeByte = _mmu.read8(_registers.pc++);
			if (_opCodeByte == 0xCB)
			{
				_state = State::FetchExt;
			}
			else if (_opCodeByte == 0x76)
			{
				if (_interrupts.isHaltBug())
				{
					std::cout << "HALT BUG" << std::endl;
					_state = State::Fetch;
					_haltBug = true;
					return;
				}
				else
				{
					//std::cout << "HALT" << std::endl;
					_state = State::Halt;
					return;
				}
			}
			else
			{
				_currentOpCode = &_instructions[_opCodeByte];
				_state = State::Execute;
			}

			if (_haltBug)
			{
				_registers.pc--;
				_haltBug = false;
			}
			break;
		}
		case State::FetchExt:
		{
			_opCodeByte = _mmu.read8(_registers.pc++);
			_currentOpCode = &_instructionsCB[_opCodeByte];
			_state = State::Execute;
			return;
		}
		case State::Execute:
		{
			if (_currentOpCode->execute(_currentInstruction++))
			{
				_currentInstruction = 0;
				_state = State::Fetch;
			}
			return;
		}
		case State::Halt:
			if (_interrupts.hasPendingInterrupts())
			{
				_state = State::Fetch;
			}
			return;
		case State::Interrupt:
		{
			Interrupts::Handler* interruptHandler = _interrupts.handleInterrupts();
			if (interruptHandler != nullptr)
			{
				_currentOpCode = interruptServiceRoutine(interruptHandler->addr);
				_interrupts.clearInterrupt(interruptHandler->type);
				_interrupts.setIme(false);
				_state = State::Execute;
			}
			else
			{
				_state = State::Fetch;
			}
			break;
		}
		default:
			break;
		}
	}
}

CPU::OpCode* CPU::interruptServiceRoutine(uint16_t addr)
{
	_isr.value = addr & 0xFF;
	return &_isr;
}