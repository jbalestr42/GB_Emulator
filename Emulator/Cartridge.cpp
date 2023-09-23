#include "Cartridge.hpp"
#include "MMU.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <array>

Cartridge::Cartridge() :
	_romBegin(0x0000),
	_romEnd(0x7FFF),
	_title(nullptr),
	_type(Cartridge::Type::ROM_ONLY),
	_romSize(0),
	_ramSize(0)
{ }

void Cartridge::init()
{
	_title = (char*)&_data[Cartridge::TITLE_ADDR];
	_type = static_cast<Cartridge::Type>(read8(Cartridge::CARTRIDGE_TYPE_ADDR));
	_romSize = 32768 * (1 << read8(Cartridge::ROM_SIZE_ADDR));

	std::array<size_t, 6> ramSize = { 0, 0, 8192, 32768, 131072, 65536 };
	_ramSize = hasRam() ? ramSize[read8(Cartridge::RAM_SIZE_ADDR)] : 0;
}

bool Cartridge::hasRam() const
{
	std::set<Cartridge::Type> typesWithRam = { MBC1_RAM, MBC1_RAM_BATTERY };
	return typesWithRam.count(_type) > 0;
}

bool Cartridge::loadRom(const char* path)
{
	std::ifstream file(path, std::fstream::in | std::ios::binary);

	if (!file.good())
	{
		return false;
	}

	_data = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	init();

	return true;
}

uint8_t Cartridge::read8(uint16_t addr)
{
	return _data[addr];
}

void Cartridge::write8(uint16_t addr, uint8_t v)
{
	_data[addr] = v;
}

bool Cartridge::isInRange(uint16_t addr) const
{
	return addr >= _romBegin && addr <= _romEnd;
}

size_t Cartridge::size() const
{
	return _romEnd - _romBegin + 1;
}

const char* Cartridge::name() const
{
	return "Cartridge";
}