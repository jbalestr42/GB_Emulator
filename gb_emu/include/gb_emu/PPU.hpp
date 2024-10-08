#pragma once

#include <cstdint>
#include "OamSearchMode.hpp"

class MMU;
class Interrupts;
class IDisplay;

class PPU
{
public:
	enum Mode
	{
		OamSearch = 2,
		PixelTransfer = 3,
		HBlank = 0,
		VBlank = 1
	};

	PPU(MMU& mmu, Interrupts& interrupts, IDisplay& display);
	~PPU() = default;

	void initialize();
	void tick();

	static const size_t OAM_TICKS = 80;
	static const size_t PIXEL_TRANSFER_TICKS = 172;
	static const size_t HBLANK_TICKS = 204;
	static const size_t VBLANK_TICKS = 456;
	static const size_t SCANLINE_PER_FRAME = 154;
	static const size_t TICKS_PER_FRAME = SCANLINE_PER_FRAME * VBLANK_TICKS;

private:
	void draw();
	void drawBackground();
	void drawWindow();
	void drawSprites();
	void setMode(PPU::Mode mode);
	uint16_t getTileAddr(uint16_t tileMapAddr) const;
	uint8_t getColorId(uint16_t tileAddr, uint8_t xTileCoord, uint8_t yTileCoord) const;
	uint8_t getColor(uint8_t colorId, uint16_t paletteAddr) const;
	bool isLYLYCInterruptEnabled() const;
	bool isHBlankInterruptEnabled() const;
	bool isVBlankInterruptEnabled() const;
	bool isOamInterruptEnabled() const;

	static const uint16_t VRAM_TILEDATA_0_ADDR = 0x8000;
	static const uint16_t VRAM_TILEDATA_1_ADDR = 0x8800;
	static const uint16_t VRAM_TILEDATA_2_ADDR = 0x9000;

	static const uint16_t VRAM_TILEMAP_1_ADDR = 0x9800;
	static const uint16_t VRAM_TILEMAP_2_ADDR = 0x9C00;

	static const uint8_t LCDC_BG_WIN_PRIORITY_ENABLE_POS = 0;
	static const uint8_t LCDC_OBJ_ENABLE_POS = 1;
	static const uint8_t LCDC_OBJ_SIZE_POS = 2;
	static const uint8_t LCDC_BG_TILEMAP_POS = 3;
	static const uint8_t LCDC_BG_WIN_DATA_POS = 4;
	static const uint8_t LCDC_WIN_ENABLE_POS = 5;
	static const uint8_t LCDC_WIN_TILEMAP_POS = 6;
	static const uint8_t LCDC_LCD_PPU_ENABLE_POS = 7;

	static const size_t TILEMAP_WIDTH_TILE = 32;
	static const size_t TILEMAP_HEIGHT_TILE = 32;
	static const size_t TILE_WIDTH_PX = 8;
	static const size_t TILE_HEIGHT_PX = 8;
	static const size_t TILEMAP_WIDTH_PX = TILEMAP_WIDTH_TILE * TILE_WIDTH_PX;
	static const size_t TILEMAP_HEIGHT_PX = TILEMAP_HEIGHT_TILE * TILE_HEIGHT_PX;
	static const uint16_t BYTES_PER_TILE = 16;

	static const uint8_t WIDTH = 160;
	static const uint8_t HEIGHT = 144;

	MMU& _mmu;
	Interrupts& _interrupts;
	IDisplay& _display;

	PPU::Mode _mode;
	OamSearchMode _oamSearchMode;
	size_t _ticks;
	uint8_t _currentLine;
	uint8_t _windowLineCounter;
	bool _lycInterruptRaiseDuringRendering;
};