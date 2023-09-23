#include "MBC.hpp"
#include "Cartridge.hpp"

MBC::MBC(Cartridge& cartridge, uint16_t ramBegin, uint16_t ramEnd) :
	_cartridge(cartridge),
	_ram("Cartridge RAM", ramBegin, ramEnd)
{}

uint8_t MBC::read8(uint16_t addr)
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

void MBC::write8(uint16_t addr, uint8_t v)
{
	if (_cartridge.isInRange(addr))
	{
		_cartridge.write8(addr, v);
	}

	if (_ram.isInRange(addr))
	{
		_ram.write8(addr, v);
	}
}

bool MBC::isInRange(uint16_t addr) const
{
	return _cartridge.isInRange(addr) || _ram.isInRange(addr);
}

size_t MBC::size() const
{
	return _cartridge.size() + _ram.size();
}

const char* MBC::name() const
{
	return "MBC";
}