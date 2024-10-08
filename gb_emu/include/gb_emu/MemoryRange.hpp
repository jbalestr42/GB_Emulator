#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "IMemoryRange.hpp"

class MemoryRange : public IMemoryRange
{
public:
	MemoryRange();
	MemoryRange(std::string name, uint16_t begin, uint16_t end);
	~MemoryRange() = default;

	virtual uint8_t read8(size_t addr) override;
	virtual void write8(size_t addr, uint8_t v) override;
	virtual bool isInRange(size_t addr) const override;
	virtual size_t size() const override;
	virtual const char* name() const override;

private:
	std::string _name;
	uint16_t _begin;
	uint16_t _end;
	std::vector<uint8_t> _data;
};