#pragma once

#include <map>
#include <vector>
#include <functional>
#include <fstream>
#include "MMU.hpp"
#include "BitUtils.hpp"
#include "Interrupts.hpp"
#include "Registers.hpp"

class CPU
{
private:
	struct OpCode
	{
		const char* mnemonic;
		uint8_t value;
		uint8_t size;
		uint8_t tcycles; // T - states: 4, 8, 12, 16
		uint8_t mcycles; // M - cycles: 1, 2, 3, 4
		std::vector<std::function<void()>> steps;

		OpCode() :
			OpCode("", 0, 0, 0, {})
		{}

		OpCode(const char* mnemonic, uint8_t value, uint8_t size, uint8_t tcycles, std::vector<std::function<void()>> steps) :
			mnemonic(mnemonic),
			value(value),
			size(size),
			tcycles(tcycles),
			mcycles(tcycles / 4),
			steps(steps)
		{}
	};

	static const uint16_t START_PC_ADDR = 0x0100;

public:
	CPU(MMU& mmu, Interrupts& interrupts);
	~CPU() = default;

	void initInstructions();
	uint8_t fetchInstruction();
	CPU::OpCode & disassembleInstruction(uint8_t opCode, bool isPrefixCB);
	size_t update();

	static const int CLOCK_FREQUENCY_HZ = 4194304;

private:
	void interruptServiceRoutine(uint16_t addr);

	struct StepData
	{
		uint8_t msb;
		uint8_t lsb;
		uint16_t addr;
		uint16_t u16;
		uint8_t overrideCycles;
	};

	StepData _data;

	MMU& _mmu;
	Interrupts& _interrupts;
	Registers _registers;
	std::map<uint8_t, OpCode> _instructions;
	std::map<uint8_t, OpCode> _instructionsCB;
	bool _halt;
	bool _haltBug;
	bool _interruptEnableRequest;

	int count = 1;
};