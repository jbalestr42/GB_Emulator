#pragma once

#include <SFML\Graphics.hpp>
#include <cstdint>
#include "OamSearchMode.hpp"

class MMU;
class Interrupts;

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

	PPU(MMU& mmu, Interrupts& interrupts, std::uint16_t width, std::uint16_t height, std::uint8_t pixelSize, std::uint8_t frameRate, const char* title);
	~PPU() = default;

	void tick();
	void display();
	void clear();
	void close();
	bool isOpen();
	bool pollEvent(sf::Event& events);
	void putPixel(uint8_t color, uint8_t x, uint8_t y);

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


private:
	void draw();
	void drawBackground();
	void drawWindow();
	void drawSprites();
	void setMode(PPU::Mode mode);
	bool isLYLYCInterruptEnabled() const;
	bool isHBlankInterruptEnabled() const;
	bool isVBlankInterruptEnabled() const;
	bool isOamInterruptEnabled() const;
	sf::Color getColorFromPalette(uint8_t colorId);

	MMU& _mmu;
	Interrupts& _interrupts;

	PPU::Mode _mode;
	OamSearchMode _oamSearchMode;
	size_t _ticks;
	uint8_t _currentLine;
	uint8_t _windowLineCounter;
	bool _lycInterruptRaiseDuringRendering;

	// Move to LCD/IDisplay to manage SFML and Ncurse ?
	uint16_t _width;
	uint16_t _height;
	uint8_t _pixelSize;
	uint8_t _frameRate;

	sf::RenderWindow _window;
	sf::VertexArray _vertices;
};