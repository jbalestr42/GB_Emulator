#pragma once

#include <cstdint>
#include "MemoryRange.hpp"

class Cartridge;

class MBC : public IMemoryRange
{
public:
	MBC(Cartridge& cartridge, uint16_t ramBegin, uint16_t ramEnd);
	~MBC() = default;

	virtual uint8_t read8(uint16_t addr) override;
	virtual void write8(uint16_t addr, uint8_t v) override;
	virtual bool isInRange(uint16_t addr) const override;
	virtual size_t size() const override;
	virtual const char* name() const override;

private:
	Cartridge& _cartridge;
	MemoryRange _ram;
};