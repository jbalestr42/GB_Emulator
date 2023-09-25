#pragma once
#include <cstdint>
#include "CPU.hpp"

class Timer
{
public:
	Timer(Interrupts& interrupts);
	~Timer() = default;

	static const uint16_t TICKS_TO_CYCLES = 4;
	static const uint16_t DIV_INCREMENT_RATE_HZ = 16384;
	static const uint16_t DIV_INCREMENT_RATE = CPU::CLOCK_FREQUENCY_HZ / DIV_INCREMENT_RATE_HZ;
	static const uint16_t TIMA_INCREMENT_RATES[];

	void update(size_t ticks);
	void setDivRegister();
	uint16_t getDivRegister();
	uint16_t getTima();
	void setTima(uint8_t v);
	uint8_t getTimerControl();
	void setTimerControl(uint8_t v);
	uint8_t getTma();
	void setTma(uint8_t v);

private:

	void updateDiv(size_t cycles);
	void updateTima(size_t cycles);

	Interrupts& _interrupts;

	size_t _divCycles;
	uint16_t _divRegister;

	bool _isTimaEnabled;
	uint8_t _timaRateId;
	size_t _timaCycles;
	uint8_t _tima;
	uint8_t _tma;
};