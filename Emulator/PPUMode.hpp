#pragma once

#include <cstdint>
#include "ITickable.hpp"

class PPUMode : public ITickable
{
public:
	virtual void tick(size_t ticks) = 0;
	virtual uint16_t getMaxTick() const = 0;
};