#pragma once

#include <map>
#include <vector>
#include <functional>
#include <fstream>
#include "BitUtils.hpp"
#include "Registers.hpp"

class MMU;
class Interrupts;

class CPU
{
public:
	CPU(MMU& mmu, Interrupts& interrupts);
	~CPU() = default;

	void initialize();
	void tick();
	Registers& getRegisters() { return _registers; }

	static const size_t CLOCK_FREQUENCY_HZ = 4194304;
	static const uint16_t START_PC_ADDR = 0x0100;

private:
	struct OpCode
	{
		const char* mnemonic;
		uint8_t value;
		uint8_t size;
		uint8_t tcycles; // T - states: 4, 8, 12, 16
		uint8_t mcycles; // M - cycles: 1, 2, 3, 4
		std::vector<std::function<void()>> steps;
		std::function<bool()> branching;

		OpCode() :
			OpCode("", 0, 0, 0, {}, nullptr)
		{}

		OpCode(const char* mnemonic, uint8_t value, uint8_t size, uint8_t tcycles, std::vector<std::function<void()>> steps, std::function<bool()> branching = nullptr) :
			mnemonic(mnemonic),
			value(value),
			size(size),
			tcycles(tcycles),
			mcycles(tcycles / 4),
			steps(steps),
			branching(branching)
		{}

		bool execute(uint8_t index)
		{
			// m-cycles are the minimum instructions to perform
			// if a branching is detected, the max number of instruction is steps.size()

			steps[index]();

			// No branching, execute the steps
			if (branching == nullptr)
			{
				return index == (mcycles - 1);
			}

			// Is Branch reached ?
			if (index >= (mcycles - 1))
			{
				// If Branching, keep executing otherwise stop
				return branching() ? index == (steps.size() - 1) : true;
			}

			// Branching not reached yet, execute steps
			return false;
		}
	};

	void initInstructions();
	CPU::OpCode* interruptServiceRoutine(uint16_t addr);

	enum State
	{
		Fetch,
		FetchExt,
		Execute,
		Halt,
		Interrupt
	};

	struct StepData
	{
		uint8_t msb;
		uint8_t lsb;
		uint16_t u16;
	};

	StepData _data;

	MMU& _mmu;
	Interrupts& _interrupts;
	Registers _registers;
	State _state;
	std::map<uint8_t, OpCode> _instructions;
	std::map<uint8_t, OpCode> _instructionsCB;
	bool _halt;
	bool _haltBug;
	bool _interruptEnableRequest;
	bool _interruptEnableRequestValue;
	uint8_t _opCodeByte;
	OpCode* _currentOpCode;
	uint8_t _currentInstruction;
	OpCode _isr;
	uint8_t _ticks;
};