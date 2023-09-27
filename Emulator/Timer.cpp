#include "Timer.hpp"

const uint16_t Timer::TIMA_INCREMENT_RATES[] = {
							CPU::CLOCK_FREQUENCY_HZ / 4096,
							CPU::CLOCK_FREQUENCY_HZ / 262144,
							CPU::CLOCK_FREQUENCY_HZ / 65536,
							CPU::CLOCK_FREQUENCY_HZ / 16384, };

Timer::Timer(Interrupts& interrupts) :
	_interrupts(interrupts),
	_divCycles(0),
	_div(0),
	_isTimaEnabled(false),
	_timaRateId(0),
	_timaCycles(0),
	_tima(0),
	_tma(0)
{ }

void Timer::update(size_t ticks)
{
	size_t cycles = ticks;
	updateDiv(cycles);

	if (_isTimaEnabled)
	{
		updateTima(cycles);
	}
}

void Timer::updateDiv(size_t cycles)
{
	// This register is incremented at a rate of 16384Hz. Writing any value to this register resets it to 0x00.
	// Additionally, this register is reset when executing the stop instruction, and only begins ticking again once stop mode ends.
	_divCycles += cycles;

	if (_divCycles >= DIV_INCREMENT_RATE)
	{
		_divCycles -= DIV_INCREMENT_RATE;
		_div++;
	}
}

void Timer::updateTima(size_t cycles)
{
	// This timer is incremented at the clock frequency specified by the TAC register (0xFF07).
	_timaCycles += cycles;

	uint16_t timaIncrementRate = TIMA_INCREMENT_RATES[_timaRateId];
	while (_timaCycles >= timaIncrementRate)
	{
		_timaCycles -= timaIncrementRate;
		if (_tima == 0xFF)
		{
			// When the value overflows it is reset to the value specified in TMA and an interrupt is requested
			_tima = _tma;
			_interrupts.raiseInterrupt(Interrupts::Type::Timer);
		}
		else
		{
			_tima++;
		}
	}
}

uint16_t Timer::getDiv()
{
	return _div;
}

void Timer::setDiv()
{
	_div = 0;
	_timaCycles = 0;
}

uint16_t Timer::getTima()
{
	return _tima;
}

void Timer::setTima(uint8_t v)
{
	_tima = v;
}

uint8_t Timer::getTac()
{
	return 0b11111000 | _timaRateId | _isTimaEnabled << 2;
}

void Timer::setTac(uint8_t v)
{
	_isTimaEnabled = BitUtils::GetBit(v, 2);
	
	_timaRateId = v & 0b00000011;
	_timaCycles = 0;
}

uint8_t Timer::getTma()
{
	return _tma;
}

void Timer::setTma(uint8_t v)
{
	_tma = v;
}