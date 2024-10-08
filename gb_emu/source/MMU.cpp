#include "MMU.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

MMU::MMU()
{ }

MMU::~MMU()
{
	for (size_t i = 0; i < _memoryRanges.size(); i++)
	{
		delete _memoryRanges[i];
	}
}

void MMU::initialize()
{
	// 0000 - 3FFF | 16 KiB ROM bank 00 | From cartridge, usually a fixed bank
	// 4000 - 7FFF | 16 KiB ROM Bank 01~NN | From cartridge, switchable bank via mapper (if any)
	_memoryRanges.push_back(_cartridge.createMBC());
	// 8000 - 9FFF | 8 KiB Video RAM (VRAM) | In CGB mode, switchable bank 0 / 1
	// A000 - BFFF | 8 KiB External RAM | From cartridge, switchable bank (if any)
	_memoryRanges.push_back(new MemoryRange("VRAM", 0x8000, 0x9FFF));
	// C000 - CFFF | 4 KiB Work RAM (WRAM)
	// D000 - DFFF | 4 KiB Work RAM (WRAM) | In CGB mode, switchable bank 1~7
	_memoryRanges.push_back(new MemoryRange("WRAM", 0xC000, 0xDFFF));
	// E000 - FDFF | Mirror of C000~DDFF (ECHO RAM) | Nintendo says use of this area is prohibited.
	_memoryRanges.push_back(new MemoryRange("ECHO RAM", 0xE000, 0xFDFF)); // Mirror C000~DDFF from WRAM
	// FE00 - FE9F | Object attribute memory (OAM) | 40 sprites 8x8 or 8x16
	_memoryRanges.push_back(new MemoryRange("OAM", 0xFE00, 0xFE9F));
	// FEA0 - FEFF | Not Usable | Nintendo says use of this area is prohibited
	_memoryRanges.push_back(new MemoryRange("Prohibited", 0xFEA0, 0xFEFF));
	// FF00 - FF7F | I/O Registers
	_memoryRanges.push_back(new MemoryRange("I/O Registers", 0xFF00, 0xFF7F));
	// FF80 - FFFE | High RAM (HRAM)
	_memoryRanges.push_back(new MemoryRange("HRAM", 0xFF80, 0xFFFE));
	// FFFF - FFFF | Interrupt Enable register (IE)
	_memoryRanges.push_back(new MemoryRange("IE Register", 0xFFFF, 0xFFFF));
}

uint8_t MMU::read8(size_t addr)
{
	if (_memoryOverride.count(addr) != 0 && _memoryOverride[addr].read8 != nullptr)
	{
		return _memoryOverride[addr].read8();
	}

	for (IMemoryRange* memoryRange : _memoryRanges)
	{
		if (memoryRange->isInRange(addr))
		{
			return memoryRange->read8(addr);
		}
	}

	return 0xFF;
}

void MMU::write8(size_t addr, uint8_t v)
{
	if (_memoryOverride.count(addr) != 0 && _memoryOverride[addr].write8 != nullptr)
	{
		_memoryOverride[addr].write8(v);
	}
	else
	{
		for (IMemoryRange* memoryRange : _memoryRanges)
		{
			if (memoryRange->isInRange(addr))
			{
				memoryRange->write8(addr, v);
				return;
			}
		}
	}
}

bool MMU::isInRange(size_t addr) const
{
	for (IMemoryRange* memoryRange : _memoryRanges)
	{
		if (memoryRange->isInRange(addr))
		{
			return true;
		}
	}
	return false;
}

size_t MMU::size() const
{
	size_t size = 0;
	for (IMemoryRange* memoryRange : _memoryRanges)
	{
		size += memoryRange->size();
	}
	return size;
}

const char* MMU::name() const
{
	return "MMU";
}

void MMU::addMemoryOverride(uint16_t addr, MemoryOverride override)
{
	_memoryOverride[addr] = override;
}

bool MMU::loadRom(const char* path)
{
	return _cartridge.loadRom(path);
}

Cartridge& MMU::getCartridge()
{
	return _cartridge;
}

void MMU::dump()
{
	std::stringstream ss;

	ss << std::hex << std::setfill('0');

	for (size_t i = 0; i < size(); i++)
	{
		if (i % 16 == 0)
		{
			ss << std::endl << std::hex << std::setfill('0') << std::setw(4) << i << " ";
		}
		if (i % 2 == 0)
		{
			ss << " ";
		}
		ss << std::hex << std::setw(2) << static_cast<int>(read8(static_cast<uint16_t>(i)));
	}

	std::cout << ss.str() << std::endl;
}