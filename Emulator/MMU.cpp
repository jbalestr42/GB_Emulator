#include "MMU.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

MMU::MMU()
{
	std::memset(_memory, 0, MMU::MEMORY_SIZE);
}

uint8_t MMU::read8(size_t p)
{
	if (_memoryOverride.count(p) != 0 && _memoryOverride[p].read8 != nullptr)
	{
		std::cout << "override read8 "<< std::hex << p << std::endl;
		return _memoryOverride[p].read8();
	}

	return _memory[p];
}

void MMU::write8(size_t p, uint8_t v)
{
	if (_memoryOverride.count(p) != 0 && _memoryOverride[p].write8 != nullptr)
	{
		std::cout << "override write8 " << std::hex << p << " " << (int)v << std::endl;
		_memoryOverride[p].write8(v);
	}
	else
	{
		if (p == 0xFF0F)
		{
			std::cout << "write IF 0xff0f : " << (int)v << std::endl;
		}
		if (p == 0xFFFF)
		{
			std::cout << "write IE 0xffff : " << (int)v << std::endl;
		}
		_memory[p] = v;
	}
}

void MMU::addMemoryOverride(uint16_t addr, MemoryOverride override)
{
	_memoryOverride[addr] = override;
}

std::string uint8_to_hex_string(const uint8_t* v, const size_t s)
{
	std::stringstream ss;

	ss << std::hex << std::setfill('0');

	for (size_t i = 0; i < s; i++)
	{
		if (i % 16 == 0)
		{
			ss << std::endl;
		}
		if (i % 2 == 0)
		{
			ss << " ";
		}
		ss << std::hex << std::setw(2) << static_cast<int>(v[i]);
	}

	return ss.str();
}

void dump(const uint8_t* v, const size_t s)
{
	for (size_t i = 0; i < s; i++)
	{
		std::cout << v[i] << std::endl;
	}
}

void MMU::dump() const
{
	std::cout << uint8_to_hex_string(_memory, MMU::MEMORY_SIZE) << std::endl;
}

uint8_t MMU::loadRom(const char* path)
{
	std::ifstream file(path, std::fstream::in | std::ios::binary);

	if (file.is_open())
	{
		file.read((char*)&_memory, sizeof(uint8_t) * MMU::MEMORY_SIZE);
		file.close();
		return 1;
	}
	else
	{
		std::cout << "File not found at path " << path << std::endl;
		return 0;
	}
}