#pragma once

#include <cstdint>
#include <vector>
#include "IMemoryRange.hpp"

class MemoryRange : public IMemoryRange
{
public:
	MemoryRange();
	MemoryRange(const char * name, uint16_t begin, uint16_t end);
	~MemoryRange() = default;

	virtual uint8_t read8(uint16_t addr) override;
	virtual void write8(uint16_t addr, uint8_t v) override;
	virtual bool isInRange(uint16_t addr) const override;
	virtual size_t size() const override;
	virtual const char* name() const override;

private:
	const char * _name;
	uint16_t _begin;
	uint16_t _end;
	std::vector<uint8_t> _data;
};