#pragma once

#include <vector>

class MMU;

class Interrupts
{
public:
	enum Type
	{
		VBlank = 0,
		LCDStat = 1,
		Timer = 2,
		Serial = 3,
		Joypad = 4
	};

	struct Handler
	{
		const char* name;
		size_t bit;
		uint8_t addr;
		Interrupts::Type type;

		Handler(const char* name, size_t bit, uint8_t addr, Interrupts::Type type) :
			name(name),
			bit(bit),
			addr(addr),
			type(type)
		{}
	};

	Interrupts(MMU& mmu);
	~Interrupts() = default;

	void setIme(bool ime) { _ime = ime; }
	bool getIme() const { return _ime; }
	bool isHaltBug() const;

	Interrupts::Handler* handleInterrupts();
	void raiseInterrupt(Interrupts::Type type);
	void clearInterrupt(Interrupts::Type type);
	bool hasPendingInterrupts() const;

private:
	MMU& _mmu;
	std::vector<Handler> _handlers;
	bool _ime; // Interrupt master enable flag
};