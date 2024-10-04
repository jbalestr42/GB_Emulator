#pragma once

#include "MMU.hpp"
#include "Interrupts.hpp"
#include "CPU.hpp"
#include "Timer.hpp"
#include "PPU.hpp"
#include "Input.hpp"

class IDisplay;

class Emulator
{
public:
	Emulator(IDisplay& display);
	~Emulator() = default;

	void initialize();
	void update();

	bool loadRom(const char* path);
	void enableBootRom(bool isEnable);

private:
	IDisplay& _display;
	MMU _mmu;
	Interrupts _interrupts;
	CPU _cpu;
	Timer _timer;
	PPU _ppu;
	Input _input;

};