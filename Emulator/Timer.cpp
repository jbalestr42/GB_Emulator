#include "Timer.hpp"
#include "Interrupts.hpp"

const uint8_t Timer::FREQ_TO_BIT[] = { 9, 3, 5, 7 };

Timer::Timer(Interrupts& interrupts) :
	_interrupts(interrupts),
	_div(0),
	_tima(0),
	_tma(0),
	_tac(0),
	_previousBit(false),
	_overflow(false),
	_ticksSinceOverflow(0)
{ }

void Timer::update(size_t ticks)
{
	updateDiv((_div + ticks) & 0xFFFF);
	if (!_overflow)
	{
		return;
	}

	_ticksSinceOverflow++;
	if (_ticksSinceOverflow == 4)
	{
		_interrupts.raiseInterrupt(Interrupts::Type::Timer);
	}

	if (_ticksSinceOverflow == 5)
	{
		_tima = _tma;
	}

	if (_ticksSinceOverflow == 6)
	{
		_tima = _tma;
		_overflow = false;
		_ticksSinceOverflow = 0;
	}
}

void Timer::updateDiv(uint16_t newDiv)
{
	_div = newDiv;

	uint8_t clockSelect = _tac & 0b11;
	uint8_t bitPos = FREQ_TO_BIT[clockSelect];
	bool bit = (_div & (1 << bitPos)) != 0;
	bool timerEnable = _tac & (1 << 2);
	bit &= timerEnable;
	
	if (!bit && _previousBit)
	{
		incTima();
	}

	_previousBit = bit;
}


void Timer::incTima()
{
	_tima++;
	_tima %= 0x100;
	if (_tima == 0)
	{
		_overflow = true;
		_ticksSinceOverflow = 0;
	}
}

uint8_t Timer::getDiv()
{
	return _div >> 8;
}

void Timer::setDiv()
{
	updateDiv(0);
}

uint8_t Timer::getTima()
{
	return _tima;
}

void Timer::setTima(uint8_t v)
{
	if (_ticksSinceOverflow < 5)
	{
		_tima = v;
		_overflow = false;
		_ticksSinceOverflow = 0;
	}
}

uint8_t Timer::getTac()
{
	return 0b11111000 | _tac;
}

void Timer::setTac(uint8_t v)
{
	_tac = v;
}

uint8_t Timer::getTma()
{
	return _tma;
}

void Timer::setTma(uint8_t v)
{
	_tma = v;
}