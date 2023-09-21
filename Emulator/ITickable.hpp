#pragma once

class ITickable
{
	virtual void tick(size_t ticks) = 0;
};