#pragma once

#include "PPUMode.hpp"
#include "PixelFetcher.hpp"
#include "Fifo.hpp"

class MMU;
class PPU;

class PixelTransferMode : public PPUMode
{
public:
	PixelTransferMode(MMU& mmu, PPU& ppu);
	~PixelTransferMode() = default;

	void start();
	void tick(size_t ticks) override;
	uint16_t getMaxTick() const override { return 172; }

private:
	void startBackgroundFetch();
	void startWindowFetch();
	bool getLcdcValue(size_t pos);

	MMU& _mmu;
	PPU& _ppu;
	Fifo _fifo;
	PixelFetcher _pixelFetcher;
	uint8_t _x;
	bool _isFetchingWindow;
};