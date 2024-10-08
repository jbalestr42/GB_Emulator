#pragma once

#include <cstdint>

class MMU;

class OamSearchMode
{
public:
	struct Sprite
	{
		uint16_t addr;
		uint8_t x;
		uint8_t y;
		uint8_t tileId;
		bool paletteId;
		bool xFlip;
		bool yFlip;
		bool isBgAndWinOver;
	};

	OamSearchMode(MMU& mmu);
	~OamSearchMode() = default;

	void start();
	Sprite* getSprites() { return _sprites; }
	uint8_t getSpriteCount() const { return _currentSprite; }

	static const size_t MAX_SPRITE = 10;
	static const uint16_t VRAM_OAM_ADDR = 0xFE00;

private:
	MMU& _mmu;
	uint8_t _currentSprite;
	Sprite _sprites[40];
};