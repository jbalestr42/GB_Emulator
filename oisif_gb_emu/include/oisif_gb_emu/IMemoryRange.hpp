#pragma once

class IMemoryRange
{
public:
	virtual uint8_t read8(size_t addr) = 0;
	virtual void write8(size_t addr, uint8_t v) = 0;
	virtual bool isInRange(size_t addr) const = 0;
	virtual size_t size() const = 0;
	virtual const char* name() const = 0;
};