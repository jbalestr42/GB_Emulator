#include "MBC3.hpp"
#include "Cartridge.hpp"
#include <math.h>

MBC3::MBC3(Cartridge& cartridge) :
	_cartridge(cartridge),
	_isRamEnabled(false),
	_isRtcEnabled(false),
	_romBankId(1),
	_ramBankId(0)
{
	_ram.resize(_cartridge.getRamBankCount() * Cartridge::RAM_BANK_SIZE, 0);
}

uint8_t MBC3::read8(size_t addr)
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
		if (_isRtcEnabled)
		{
			// TODO
			return 0x00;
		}
		else
		{
			return _ram[addr - 0xA000 + (_ramBankId * Cartridge::RAM_BANK_SIZE)];
		}
	}

	return 0xFF;
}

void MBC3::write8(size_t addr, uint8_t v)
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
		// This 7-bit register selects the ROM bank id for the 4000–7FFF region
		_romBankId = std::max(v & 0x7F, 0x01);

		//int count = _cartridge.getRomBankCount();
		//int nbBits = static_cast<uint8_t>(log2(count));
		//uint8_t mask = (1 << nbBits) - 1;
		//_romBankId &= mask;
	}

	// RAM Bank Id or RTC Register Select
	else if (addr >= 0x4000 && addr <= 0x5FFF)
	{
		// As for the MBC1s RAM Banking Mode, writing a value in range for 0x00-0x03 maps 
		// the corresponding external RAM Bank (if any) into memory at A000-BFFF
		if (v < 4)
		{
			_isRtcEnabled = false;
			_ramBankId = (v & 0b00000011);
			_ramBankId = _cartridge.hasRam() ? _ramBankId % _cartridge.getRamBankCount() : _ramBankId;
		}

		// When writing a value of 0x08-0x0C, this will map the corresponding RTC register into memory at A000-BFFF
		else
		{
			_isRtcEnabled = true;
		}
	}

	// Latch Clock Data
	else if (addr >= 0x6000 && addr <= 0x7FFF)
	{
	}
	else if (addr >= 0xA000 && addr <= 0xBFFF && _isRamEnabled)
	{
		// Access RTC register
		if (_isRtcEnabled)
		{
			// TODO
		}

		// Access RAM
		else
		{
			_ram[addr - 0xA000 + (_ramBankId * Cartridge::RAM_BANK_SIZE)] = v;
		}
	}
}

bool MBC3::isInRange(size_t addr) const
{
	return _cartridge.isInRange(addr) || (addr >= 0xA000 && addr <= 0xBFFF);
}

size_t MBC3::size() const
{
	return _cartridge.size() + Cartridge::RAM_BANK_SIZE;
}

const char* MBC3::name() const
{
	return "MBC3";
}