#include "Cartridge.hpp"
#include "MMU.hpp"
#include "MBCRomOnly.hpp"
#include "MBC1.hpp"
#include "MBC2.hpp"
#include "MBC3.hpp"
#include "MBC5.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <array>

Cartridge::Cartridge() :
	_romBegin(0x0000),
	_romEnd(0x7FFF),
	_title(nullptr),
	_type(Cartridge::Type::ROM_ONLY),
	_romSize(0),
	_ramSize(0),
	_isMulticart(false)
{ }

bool Cartridge::loadRom(const char* path)
{
	std::ifstream file(path, std::fstream::in | std::ios::binary);

	if (!file.good())
	{
		std::cout << "File not found at path '" << path << "'" << std::endl;
		return false;
	}

	_data = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	init();

	return true;
}

void Cartridge::init()
{
	_title = (char*)&_data[Cartridge::TITLE_ADDR];
	_type = getRomTypeFromId(read8(Cartridge::CARTRIDGE_TYPE_ADDR));
	uint8_t romSizeId = read8(Cartridge::ROM_SIZE_ADDR);
	_romSize = 32768 * (1 << romSizeId);

	std::array<size_t, 6> ramSize = { 0, 0, 8192, 32768, 131072, 65536 };
	_ramSize = hasRam() ? ramSize[read8(Cartridge::RAM_SIZE_ADDR)] : 0;

	uint8_t nintendoLogoData[48] =
	{
		0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
		0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
		0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
	};

	if (_romSize == 1048576)
	{
		_isMulticart =
			std::equal(std::begin(nintendoLogoData), std::end(nintendoLogoData), &_data[0x10 * 0x4000 + 0x104]) &&
			std::equal(std::begin(nintendoLogoData), std::end(nintendoLogoData), &_data[0x20 * 0x4000 + 0x104]);
	}
}

bool Cartridge::hasRam() const
{
	std::set<Cartridge::Type> typesWithRam = {	MBC_1_RAM,
												MBC_1_RAM_BATTERY,
												MBC_ROM_RAM,
												MBC_ROM_RAM_BATTERY,
												MMM_01_RAM,
												MMM_01_RAM_BATTERY,
												MBC_3_RAM,
												MBC_3_RAM_BATTERY,
												MBC_5_RAM,
												MBC_5_RAM_BATTERY,
												MBC_5_RUMBLE_RAM,
												MBC_5_RUMBLE_RAM_BATTERY,
												MBC_7_SENSOR_RUMBLE_RAM_BATTERY,
												HUC1_RAM_BATTERY };
	return typesWithRam.count(_type) > 0;
}

Cartridge::Type Cartridge::getRomTypeFromId(uint8_t typeId) const
{
	switch (typeId)
	{
		case 0x00:
			return ROM_ONLY;
		case 0x01:
			return MBC_1;
		case 0x02:
			return MBC_1_RAM;
		case 0x03:
			return MBC_1_RAM_BATTERY;
		case 0x05:
			return MBC_2;
		case 0x06:
			return MBC_2_BATTERY;
		case 0x08:
			return MBC_ROM_RAM;
		case 0x09:
			return MBC_ROM_RAM_BATTERY;
		case 0x0B:
			return MMM_01;
		case 0x0C:
			return MMM_01_RAM;
		case 0x0D:
			return MMM_01_RAM_BATTERY;
		case 0x0F:
			return MBC_3_TIMER_BATTERY;
		case 0x11:
			return MBC_3;
		case 0x12:
			return MBC_3_RAM;
		case 0x13:
			return MBC_3_RAM_BATTERY;
		case 0x19:
			return MBC_5;
		case 0x1A:
			return MBC_5_RAM;
		case 0x1B:
			return MBC_5_RAM_BATTERY;
		case 0x1C:
			return MBC_5_RUMBLE;
		case 0x1D:
			return MBC_5_RUMBLE_RAM;
		case 0x1E:
			return MBC_5_RUMBLE_RAM_BATTERY;
		case 0x20:
			return MBC_6;
		case 0x22:
			return MBC_7_SENSOR_RUMBLE_RAM_BATTERY;
		case 0xFC:
			return POCKET_CAMERA;
		case 0xFD:
			return BANDAI_TAMA5;
		case 0xFE:
			return HUC3;
		case 0xFF:
			return HUC1_RAM_BATTERY;
		default:
			return ROM_ONLY;
	}
}

uint8_t Cartridge::read8(size_t addr)
{
	return _data[addr];
}

void Cartridge::write8(size_t addr, uint8_t v)
{
	_data[addr] = v;
}

bool Cartridge::isInRange(size_t addr) const
{
	return addr >= _romBegin && addr <= _romEnd;
}

size_t Cartridge::size() const
{
	return _romEnd - _romBegin + 1;
}

const char* Cartridge::name() const
{
	return "Cartridge";
}

IMemoryRange* Cartridge::createMBC()
{
	switch (_type)
	{
	case Cartridge::ROM_ONLY:
		return new MBCRomOnly(*this);
	case Cartridge::MBC_1:
	case Cartridge::MBC_1_RAM:
	case Cartridge::MBC_1_RAM_BATTERY:
		return new MBC1(*this);
	case Cartridge::MBC_2:
	case Cartridge::MBC_2_BATTERY:
		return new MBC2(*this);
	case Cartridge::MBC_3_TIMER_BATTERY:
	case Cartridge::MBC_3:
	case Cartridge::MBC_3_RAM:
	case Cartridge::MBC_3_RAM_BATTERY:
		return new MBC3(*this);
	case Cartridge::MBC_5:
	case Cartridge::MBC_5_RAM:
	case Cartridge::MBC_5_RAM_BATTERY:
	case Cartridge::MBC_5_RUMBLE:
	case Cartridge::MBC_5_RUMBLE_RAM:
	case Cartridge::MBC_5_RUMBLE_RAM_BATTERY:
		return new MBC5(*this);
	default:
		std::cout << "MBC type '" << _type << "' not managed. Using MBCRomOnly" << std::endl;
		return new MBCRomOnly(*this);
	}
	return nullptr;
}