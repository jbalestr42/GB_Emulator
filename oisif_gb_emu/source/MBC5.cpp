#include "MBC5.hpp"
#include "Cartridge.hpp"

MBC5::MBC5(Cartridge& cartridge) :
	_cartridge(cartridge),
	_isRamEnabled(false),
	_romBankId(1),
	_ramBankId(0)
{
	_ram.resize(_cartridge.getRamBankCount() * Cartridge::RAM_BANK_SIZE, 0);
}

uint8_t MBC5::read8(size_t addr)
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
		return _ram[addr - 0xA000 + (_ramBankId * Cartridge::RAM_BANK_SIZE)];
	}

	return 0xFF;
}

void MBC5::write8(size_t addr, uint8_t v)
{
	// RAM Enable Range
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// Any value with 0x0A in the lower 4 bits enables the RAM, any other value disables the RAM
		_isRamEnabled = (v & 0x0F) == 0x0A;
	}

	// ROM Bank Id - 8 least significant bits
	else if (addr >= 0x2000 && addr <= 0x2FFF)
	{
		// The 8 least significant bits of the ROM bank number go here
		_romBankId = (_romBankId & 0x100) | v;
	}

	// ROM Bank Id - 9th bit
	else if (addr >= 0x3000 && addr <= 0x3FFF)
	{
		// The 9th bit of the ROM bank number goes here
		uint16_t value = v & 0x01;
		_romBankId = (_romBankId & 0xFF) | (value << 8);
	}

	// RAM Bank Id or RTC Register Select
	else if (addr >= 0x4000 && addr <= 0x5FFF)
	{
		// As for the MBC1s RAM Banking Mode, writing a value in range for 0x00-0x0F maps 
		// the corresponding external RAM Bank (if any) into memory at A000-BFFF
		_ramBankId = (v & 0x0F);
	}

	// Access RAM
	else if (addr >= 0xA000 && addr <= 0xBFFF && _isRamEnabled)
	{
		_ram[addr - 0xA000 + (_ramBankId * Cartridge::RAM_BANK_SIZE)] = v;
	}
}

bool MBC5::isInRange(size_t addr) const
{
	return _cartridge.isInRange(addr) || (addr >= 0xA000 && addr <= 0xBFFF);
}

size_t MBC5::size() const
{
	return _cartridge.size() + Cartridge::RAM_BANK_SIZE;
}

const char* MBC5::name() const
{
	return "MBC5";
}