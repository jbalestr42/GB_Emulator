#include "MBC2.hpp"
#include "Cartridge.hpp"
#include <math.h>

MBC2::MBC2(Cartridge& cartridge) :
	_cartridge(cartridge),
	_isRamEnabled(false),
	_romBankId(1)
{
	_ram.resize(512);
}

uint8_t MBC2::read8(size_t addr)
{
	if (addr >= 0x0000 && addr <= 0x3FFF)
	{
		return _cartridge.read8(addr);
	}
	else if (addr >= 0x4000 && addr <= 0x7FFF)
	{
		return _cartridge.read8((addr - 0x4000 + (_romBankId * Cartridge::ROM_BANK_SIZE)) % _cartridge.getRomSize());
	}
	else if (addr >= 0xA000 && addr <= 0xBFFF && _isRamEnabled)
	{
		return _ram[addr & 0x01FF];
	}

	return 0xFF;
}

void MBC2::write8(size_t addr, uint8_t v)
{
	if (addr >= 0x0000 && addr <= 0x3FFF)
	{
		// RAM Enable Range
		if ((addr & 0x0100) == 0)
		{
			// Any value with 0x0A in the lower 4 bits enables the RAM, any other value disables the RAM
			_isRamEnabled = (v & 0x0F) == 0x0A;
		}
		// ROM Bank Id
		else
		{
			// The lower 4 bits of the value written to this address range specify the ROM bank number
			_romBankId = std::max(v & 0x0F, 0x01);
		}
	}
	else if (addr >= 0xA000 && addr <= 0xBFFF && _isRamEnabled)
	{
		_ram[addr & 0x01FF] = v | 0xF0;
	}
}

bool MBC2::isInRange(size_t addr) const
{
	return _cartridge.isInRange(addr) || (addr >= 0xA000 && addr <= 0xBFFF);
}

size_t MBC2::size() const
{
	return _cartridge.size() + Cartridge::RAM_BANK_SIZE;
}

const char* MBC2::name() const
{
	return "MBC2";
}