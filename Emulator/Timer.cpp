#include "Timer.hpp"

const uint16_t Timer::TIMER_COUNTER_INCREMENT_RATES[] = { 
							CPU::CLOCK_FREQUENCY_HZ / 4096,
							CPU::CLOCK_FREQUENCY_HZ / 262144,
							CPU::CLOCK_FREQUENCY_HZ / 65536,
							CPU::CLOCK_FREQUENCY_HZ / 16384, };

Timer::Timer(Interrupts& interrupts) :
	_interrupts(interrupts),
	_divCycles(0),
	_divRegister(0),
	_isTimerCounterEnabled(false),
	_timerCounterRateId(0),
	_timerCounterCycles(0),
	_timerCounter(0),
	_timerModulo(0)
{ }

void Timer::update(size_t ticks)
{
	size_t cycles = ticks * TICKS_TO_CYCLES;
	updateDiv(cycles);

	if (_isTimerCounterEnabled)
	{
		updateTimeCounter(cycles);
	}
}

void Timer::updateDiv(size_t cycles)
{
	_divCycles += cycles;

	if (_divCycles >= DIV_INCREMENT_RATE)
	{
		_divCycles -= DIV_INCREMENT_RATE;
		_divRegister++;
	}
}

void Timer::updateTimeCounter(size_t cycles)
{
	_timerCounterCycles += cycles;

	uint16_t timerCounterIncrementRate = TIMER_COUNTER_INCREMENT_RATES[_timerCounterRateId];
	while (_timerCounterCycles >= timerCounterIncrementRate)
	{
		_timerCounterCycles -= timerCounterIncrementRate;
		if (_timerCounter == 0xFF)
		{
			_timerCounter = _timerModulo;
			_interrupts.raiseInterrupt(Interrupts::Type::Timer);
		}
		else
		{
			_timerCounter++;
		}
	}
}

uint16_t Timer::getDivRegister()
{
	return _divRegister;
}

uint16_t Timer::getTimerCounter()
{
	return _timerCounter;
}

void Timer::setTimerCounter(uint8_t v)
{
	_timerCounter = v;
}

uint8_t Timer::getTimerControl()
{
	return 0b11111000 | _timerCounterRateId | _isTimerCounterEnabled << 2;
}

void Timer::setTimerControl(uint8_t v)
{
	_isTimerCounterEnabled = BitUtils::GetBit(v, 2);
	
	_timerCounterRateId = v & 0b00000011;
	if (_timerCounterRateId >= 4)
	{
		return;
	}

	_timerCounterCycles = 0;
}

uint8_t Timer::getTimerModulo()
{
	return _timerModulo;
}

void Timer::setTimerModulo(uint8_t v)
{
	_timerModulo = v;
}

void Timer::resetDivRegister()
{
	_divRegister = 0;
	_timerCounterCycles = 0;
}