#pragma once

#include "PPUMode.hpp"

class VBlankMode : public PPUMode
{
public:
	void tick(size_t ticks) {}
	uint16_t getMaxTick() const { return 456; }
};