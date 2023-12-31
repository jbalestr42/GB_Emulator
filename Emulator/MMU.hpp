#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <vector>
#include "Cartridge.hpp"
#include "MemoryRange.hpp"

class MMU : public IMemoryRange
{
public:
	struct MemoryOverride
	{
		std::function<uint8_t()> read8;
		std::function<void(uint8_t)> write8;

		MemoryOverride() :
			read8(nullptr),
			write8(nullptr) {}

		MemoryOverride(std::function<uint8_t()> read8, std::function<void(uint8_t)> write8) :
			read8(read8),
			write8(write8) {}
	};

	MMU();
	~MMU();
	
	void initialize();

	virtual uint8_t read8(size_t addr) override;
	virtual void write8(size_t addr, uint8_t v) override;
	virtual bool isInRange(size_t addr) const override;
	virtual size_t size() const override;
	virtual const char* name() const override;
	bool loadRom(const char* path);
	Cartridge& getCartridge();

	void dump();
	void addMemoryOverride(uint16_t addr, MemoryOverride override);

private:
	std::vector<IMemoryRange*> _memoryRanges;
	std::map<size_t, MemoryOverride> _memoryOverride;
	Cartridge _cartridge;

};