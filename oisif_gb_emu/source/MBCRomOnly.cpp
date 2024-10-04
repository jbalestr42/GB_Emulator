#include "MBCRomOnly.hpp"
#include "Cartridge.hpp"

MBCRomOnly::MBCRomOnly(Cartridge& cartridge) :
	_cartridge(cartridge),
	_ram("Cartridge RAM", 0xA000, 0xBFFF)
{}

uint8_t MBCRomOnly::read8(size_t addr)
{
	if (_cartridge.isInRange(addr))
	{
		return _cartridge.read8(addr);
	}

	if (_ram.isInRange(addr))
	{
		return _ram.read8(addr);
	}

	return 0xFF;
}

void MBCRomOnly::write8(size_t addr, uint8_t v)
{
	// Only allowed to write in RAM
	if (_ram.isInRange(addr))
	{
		_ram.write8(addr, v);
	}
}

bool MBCRomOnly::isInRange(size_t addr) const
{
	return _cartridge.isInRange(addr) || _ram.isInRange(addr);
}

size_t MBCRomOnly::size() const
{
	return _cartridge.size() + _ram.size();
}

const char* MBCRomOnly::name() const
{
	return "MBCRomOnly";
}