#pragma once

#include <cstdint>

class Interrupts;

class Timer
{
public:
	Timer(Interrupts& interrupts);
	~Timer() = default;

	void update(size_t ticks);
	uint8_t getDiv();
	void setDiv();
	uint8_t getTima();
	void setTima(uint8_t v);
	uint8_t getTac();
	void setTac(uint8_t v);
	uint8_t getTma();
	void setTma(uint8_t v);

private:
	void incTima();
	void updateDiv(uint16_t newDiv);

	static const uint8_t FREQ_TO_BIT[];

	Interrupts& _interrupts;
	uint16_t _div;
	uint8_t _tima;
	uint8_t _tma;
	uint8_t _tac;
	bool _previousBit;
	bool _overflow;
	uint8_t _ticksSinceOverflow;
};