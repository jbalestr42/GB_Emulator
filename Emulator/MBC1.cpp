#include "MBC1.hpp"
#include "Cartridge.hpp"
#include <math.h>

MBC1::MBC1(Cartridge& cartridge) :
	_cartridge(cartridge),
	_isRamEnabled(false),
	_romBankId(1),
	_romBankIdHigh(0),
	_ramBankId(0),
	_bankingMode(0)
{
	_ram.resize(_cartridge.getRamBankCount() * Cartridge::RAM_BANK_SIZE, 0);
}

uint8_t MBC1::read8(size_t addr)
{
	if (addr >= 0x0000 && addr <= 0x3FFF)
	{
		addr = _bankingMode == 0 ? addr : addr + ((_romBankIdHigh * Cartridge::ROM_BANK_SIZE) % _cartridge.getRomSize());
		return _cartridge.read8(addr);
	}
	else if (addr >= 0x4000 && addr <= 0x7FFF)
	{
		return _cartridge.read8(addr - 0x4000 + (_romBankId * Cartridge::ROM_BANK_SIZE));
	}
	else if (addr >= 0xA000 && addr <= 0xBFFF && _isRamEnabled)
	{
		addr = _bankingMode == 0 ? addr - 0xA000 : addr - 0xA000 + (_ramBankId * Cartridge::RAM_BANK_SIZE);
		return _ram[addr];
	}

	return 0xFF;
}

void MBC1::write8(size_t addr, uint8_t v)
{
	// RAM Enable Range
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// Any value with 0x0A in the lower 4 bits enables the RAM, any other value disables the RAM
		_isRamEnabled = (v & 0x0F) == 0x0A;
	}

	// ROM Bank Id
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		// This 5-bit register selects the ROM bank id for the 4000–7FFF region
		if (_cartridge.isMultiCart())
		{
			_romBankId = (_romBankId & 0xF0) | std::max(v & 0x0F, 0x01);
		}
		else
		{
			_romBankId = (_romBankId & 0xE0) | std::max(v & 0x1F, 0x01);
		}
	}

	// RAM Bank Id
	else if (addr >= 0x4000 && addr <= 0x5FFF)
	{
		// This 2-bit register can be used to select a RAM Bank (32 KiB ram carts only),
		// or to specify the upper two bits (bits 5-6) of the ROM Bank number (1 MiB ROM or larger carts only).
		_ramBankId = (v & 0b00000011);
		_ramBankId = _cartridge.hasRam() ? _ramBankId % _cartridge.getRamBankCount() : _ramBankId;
		_romBankIdHigh = _ramBankId << (_cartridge.isMultiCart() ? 4 : 5);

		if (_cartridge.isMultiCart())
		{
			_romBankId = (_romBankId & 0x0F) | _romBankIdHigh;
		}
		else
		{
			_romBankId = (_romBankId & 0x1F) | _romBankIdHigh;
		}

		int count = _cartridge.getRomBankCount();
		int nbBits = static_cast<uint8_t>(log2(count));
		uint8_t mask = (1 << nbBits) - 1;
		_romBankId &= mask;
	}

	// Banking Mode Select
	else if (addr >= 0x6000 && addr <= 0x7FFF)
	{
		// This 1-bit register selects between the two MBC1 banking modes, controlling the _romBankId register
		_bankingMode = v & 0b00000001;
	}
	else if (addr >= 0xA000 && addr <= 0xBFFF && _isRamEnabled)
	{
		addr = _bankingMode == 0 ? addr - 0xA000 : addr - 0xA000 + (_ramBankId * Cartridge::RAM_BANK_SIZE);
		_ram[addr] = v;
	}
}

bool MBC1::isInRange(size_t addr) const
{
	return _cartridge.isInRange(addr) || (addr >= 0xA000 && addr <= 0xBFFF);
}

size_t MBC1::size() const
{
	return _cartridge.size() + Cartridge::RAM_BANK_SIZE;
}

const char* MBC1::name() const
{
	return "MBC1";
}