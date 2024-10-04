#pragma once

#include <cstdint>
#include "MemoryRange.hpp"

class Cartridge;

class MBCRomOnly : public IMemoryRange
{
public:
	MBCRomOnly(Cartridge& cartridge);
	~MBCRomOnly() = default;

	virtual uint8_t read8(size_t addr) override;
	virtual void write8(size_t addr, uint8_t v) override;
	virtual bool isInRange(size_t addr) const override;
	virtual size_t size() const override;
	virtual const char* name() const override;

private:
	Cartridge& _cartridge;
	MemoryRange _ram;
};