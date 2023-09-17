#include "Interrupts.hpp"
#include "HardwareRegisters.hpp"
#include "BitUtils.hpp"
#include <iostream>

Interrupts::Interrupts(MMU& mmu) :
	_mmu(mmu),
	_ime(false)
{
	_handlers.push_back(Interrupts::Handler("VBlank", 0, 0x40, Interrupts::Type::VBlank));
	_handlers.push_back(Interrupts::Handler("LCD STAT", 1, 0x48, Interrupts::Type::LCDStat));
	_handlers.push_back(Interrupts::Handler("Timer", 2, 0x50, Interrupts::Type::Timer));
	_handlers.push_back(Interrupts::Handler("Serial", 3, 0x58, Interrupts::Type::Serial));
	_handlers.push_back(Interrupts::Handler("Joypad", 4, 0x60, Interrupts::Type::Joypad));
}

Interrupts::Handler* Interrupts::handleInterrupts()
{
	if (_ime)
	{
		uint8_t ieReg = _mmu.read8(HardwareRegisters::IE_ADDR); // Enable interrupt
		uint8_t ifReg = _mmu.read8(HardwareRegisters::IF_ADDR); // Request interrupt

		for (int i = _handlers.size() - 1; i >= 0; i--)
		{
			if (BitUtils::GetBit(ieReg, _handlers[i].bit) && BitUtils::GetBit(ifReg, _handlers[i].bit))
			{
				return &_handlers[i];
			}
		}
	}
	return nullptr;
}

void Interrupts::raiseInterrupt(Interrupts::Type type)
{
	std::cout << "Interrrupts: raise interrupt " << type << std::endl;
	uint8_t ifReg = _mmu.read8(HardwareRegisters::IF_ADDR);
	_mmu.write8(HardwareRegisters::IF_ADDR, BitUtils::SetBit(ifReg, _handlers[(static_cast<size_t>(type))].bit));
}

void Interrupts::clearInterrupt(Interrupts::Type type)
{
	std::cout << "Interrrupts: clear interrupt" << std::endl;
	uint8_t ifReg = _mmu.read8(HardwareRegisters::IF_ADDR);
	_mmu.write8(HardwareRegisters::IF_ADDR, BitUtils::UnsetBit(ifReg, _handlers[(static_cast<size_t>(type))].bit));
}

bool Interrupts::hasPendingInterrupts()
{
	uint8_t ieReg = _mmu.read8(HardwareRegisters::IE_ADDR);
	uint8_t ifReg = _mmu.read8(HardwareRegisters::IF_ADDR);
	return (ieReg & ifReg) != 0;
}