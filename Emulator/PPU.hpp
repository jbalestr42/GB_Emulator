#pragma once

#include <SFML\Graphics.hpp>
#include <cstdint>
#include <vector>
#include "CPU.hpp"

class PPU
{
public:
	PPU(std::uint16_t width, std::uint16_t height, std::uint8_t pixelSize, std::uint8_t frameRate, const char* title);
	~PPU() = default;

	void display();
	void clear();
	void close();
	bool isOpen();
	bool pollEvent(sf::Event& events);
	//void draw(uint16_t b1, uint16_t b2, uint16_t b3, CPU& cpu);

	static const uint16_t VRAM_BLOCK_0_ADDR = 0x8000;
	static const uint16_t VRAM_BLOCK_1_ADDR = 0x8800;
	static const uint16_t VRAM_BLOCK_2_ADDR = 0x9000;
	static const uint16_t VRAM_TILEMAP_1_ADDR = 0x9800;
	static const uint16_t VRAM_TILEMAP_2_ADDR = 0x9C00;

	static const uint16_t VRAM_OAM_ADDR = 0xFE9F; // Object Attribute Memory


private:
	std::uint16_t _width;
	std::uint16_t _height;
	std::uint8_t _pixelSize;
	std::uint8_t _frameRate;

	sf::RenderWindow _window;
	sf::VertexArray _vertices;

	// Background position
	std::uint8_t scx;
	std::uint8_t scy;

	// Window position
	std::uint8_t wx;
	std::uint8_t wy;
};
// TIle
// 8x8 pixels
// 4 shades of grey
// 16 bytes per tile


// Background Map
// 32x32 Tiles - 256x256 pixels

// Sprites/Objtecs
// Stored in OAM
// OAM Entry: 4 bytes
// 0 - y position
// 1 - x position
// 2 - tile index
// 3 - attributes/flags (priority/palette/flip)

// Memory contains MemoryArea
// IMemoryArea
// VRAM
// RAM
// WRAM ?