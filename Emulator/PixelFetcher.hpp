#pragma once

#include <stack>
#include "ITickable.hpp"

class MMU;
class Fifo;

class PixelFetcher : public ITickable
{
public:
	enum State
	{
		GetTileId,
		GetTileDataLow,
		GetTileDataHigh,
		Push
	};

	PixelFetcher(MMU& mmu, Fifo& fifo);
	~PixelFetcher() = default;
	
	void start(uint16_t memoryAddr, uint8_t xOffset, uint8_t y);
	void tick(size_t ticks);

private:
	MMU& _mmu;
	Fifo& _fifo;
	State _state;
	size_t _ticks;
	uint16_t _memoryAddr;
	uint8_t _xOffset;
	uint8_t _y;

	uint8_t _tileId;
	uint8_t _dataLow;
	uint8_t _dataHigh;

};