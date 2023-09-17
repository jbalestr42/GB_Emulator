#pragma once

#include <cstdint>
#include <functional>
#include <map>

// rename MMU and _memory to ram
class MMU
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
	~MMU() = default;

	uint8_t loadRom(const char* path);
	void dump() const;

	uint8_t read8(size_t p);
	void write8(size_t p, uint8_t v);
	void addMemoryOverride(uint16_t addr, MemoryOverride override);

	static const uint16_t MEMORY_SIZE = 0xFFFF;

private:
	uint8_t _memory[MMU::MEMORY_SIZE];
	std::map<size_t, MemoryOverride> _memoryOverride;
};