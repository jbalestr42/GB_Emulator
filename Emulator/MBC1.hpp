#pragma once

#include <cstdint>
#include "MemoryRange.hpp"

class Cartridge;

class MBC1 : public IMemoryRange
{
public:
	MBC1(Cartridge& cartridge);
	~MBC1() = default;

	virtual uint8_t read8(size_t addr) override;
	virtual void write8(size_t addr, uint8_t v) override;
	virtual bool isInRange(size_t addr) const override;
	virtual size_t size() const override;
	virtual const char* name() const override;

private:
	Cartridge& _cartridge;
	std::vector<uint8_t> _ram;
	bool _isRamEnabled;
	uint8_t _romBankId;
	uint8_t _ramBankId;
	uint8_t _bankingMode;
};