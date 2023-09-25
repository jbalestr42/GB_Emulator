#pragma once

#include <cstdint>
#include <vector>
#include "IMemoryRange.hpp"

class MMU;

class Cartridge : public IMemoryRange
{
public:
	enum Type
	{
		ROM_ONLY,						// 0x00
		MBC_1,							// 0x01
		MBC_1_RAM,						// 0x02
		MBC_1_RAM_BATTERY,				// 0x03
		MBC_2,							// 0x05
		MBC_2_BATTERY,					// 0x06
		MBC_ROM_RAM,					// 0x08 | No licensed cartridge makes use of this option
		MBC_ROM_RAM_BATTERY,			// 0x09 | No licensed cartridge makes use of this option
		MMM_01,							// 0x0B
		MMM_01_RAM,						// 0x0C
		MMM_01_RAM_BATTERY,				// 0x0D
		MBC_3_TIMER_BATTERY,			// 0x0F
		MBC_3,							// 0x11
		MBC_3_RAM,						// 0x12
		MBC_3_RAM_BATTERY,				// 0x13
		MBC_5,							// 0x19
		MBC_5_RAM,						// 0x1A
		MBC_5_RAM_BATTERY,				// 0x1B
		MBC_5_RUMBLE,					// 0x1C
		MBC_5_RUMBLE_RAM,				// 0x1D
		MBC_5_RUMBLE_RAM_BATTERY,		// 0x1E
		MBC_6,							// 0x20
		MBC_7_SENSOR_RUMBLE_RAM_BATTERY,// 0x22
		POCKET_CAMERA,					// 0xFC
		BANDAI_TAMA5,					// 0xFD
		HUC3,							// 0xFE
		HUC1_RAM_BATTERY,				// 0xFF
	};

	Cartridge();
	~Cartridge() = default;

	bool loadRom(const char* path);
	Cartridge::Type getType() const { return _type; }
	size_t getRomSize() const { return _romSize; }
	size_t getRamSize() const { return _ramSize; }
	size_t getRomBankCount() const { return _romSize / ROM_BANK_SIZE; }
	size_t getRamBankCount() const { return _ramSize / RAM_BANK_SIZE; }
	bool isMultiCart() const { return _isMulticart; }
	bool hasRam() const;
	Cartridge::Type getRomTypeFromId(uint8_t typeId) const;

	virtual uint8_t read8(size_t addr) override;
	virtual void write8(size_t addr, uint8_t v) override;
	virtual bool isInRange(size_t addr) const override;
	virtual size_t size() const override;
	virtual const char* name() const override;

	IMemoryRange* createMBC();

	static const uint16_t TITLE_ADDR = 0x0134;
	static const uint16_t CARTRIDGE_TYPE_ADDR = 0x0147;
	static const uint16_t ROM_SIZE_ADDR = 0x0148;
	static const uint16_t RAM_SIZE_ADDR = 0x0149;
	static const uint16_t ROM_BANK_SIZE = 16384;
	static const uint16_t RAM_BANK_SIZE = 8192;

private:
	void init();

	std::vector<uint8_t> _data;
	uint16_t _romBegin;
	uint16_t _romEnd;
	char * _title;
	Cartridge::Type _type;
	size_t _romSize;
	size_t _ramSize;
	bool _isMulticart;
};