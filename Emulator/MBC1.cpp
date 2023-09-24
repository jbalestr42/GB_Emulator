#include "MBC1.hpp"
#include "Cartridge.hpp"
#include <math.h>
#include <string>

MBC1::MBC1(Cartridge& cartridge) :
	_cartridge(cartridge),
	_isRamEnabled(false),
	_romBankId(1),
	_ramBankId(0),
	_bankingMode(0)
{
	_ram.resize(_cartridge.getRamBankCount() * Cartridge::RAM_BANK_SIZE, 0);
}

uint8_t MBC1::read8(size_t addr)
{
	// TODO: On larger carts which need a >5 bit bank number, the secondary banking register at 4000–5FFF is used to supply an additional 2 bits 
	// for the effective bank number: Selected ROM Bank = (Secondary Bank << 5) + ROM Bank.1
	if (addr >= 0x0000 && addr <= 0x3FFF)
	{
		return _cartridge.read8(addr);
	}
	else if (addr >= 0x4000 && addr <= 0x7FFF)
	{
		return _cartridge.read8(addr - 0x4000 + Cartridge::ROM_BANK_SIZE * _romBankId);
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
		_romBankId = v & 0b00011111;
		if (_romBankId == 0)
		{
			_romBankId = 1;
		}

		// If the ROM Bank Number is set to a higher value than the number of banks in the cart,
		// the bank number is masked to the required number of bits
		int count = _cartridge.getRomBankCount();
		int nbBits = static_cast<uint8_t>(log2(count));
		uint8_t mask = (1 << nbBits) - 1;
		_romBankId &= mask;
	}

	// RAM Bank Id
	else if (addr >= 0x4000 && addr <= 0x5FFF)
	{
		// This 2-bit register can be used to select a RAM Bank (32 KiB ram carts only),
		// or to specify the upper two bits (bits 5-6) of the ROM Bank number (1 MiB ROM or larger carts only).
		_ramBankId = (v & 0b00000011);
		_ramBankId = _cartridge.hasRam() ? _ramBankId % _cartridge.getRamBankCount() : _ramBankId;
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