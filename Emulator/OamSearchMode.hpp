#pragma once

#include "PPUMode.hpp"

class MMU;

class OamSearchMode : public PPUMode
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
	void tick(size_t ticks) override;
	void tickAll();
	uint16_t getMaxTick() const override { return 80; }
	Sprite* getSprites() { return _sprites; }
	uint8_t getSpriteCount() const { return _currentSprite; }

	static const size_t MAX_SPRITE = 10;
	static const uint16_t VRAM_OAM_ADDR = 0xFE00;

private:
	enum State
	{
		ReadY,
		ReadX
	};

	uint8_t getSpriteHeight() const;

	MMU& _mmu;
	State _state;
	uint8_t _currentSprite;
	Sprite _sprites[40];
};