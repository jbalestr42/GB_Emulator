#pragma once

class IDisplay
{
public:
	virtual void display() = 0;
	virtual void clear() = 0;
	virtual void close() = 0;
	virtual bool isOpen() = 0;
	virtual void pollEvent() = 0;
	virtual bool isPixelWhite(uint8_t x, uint8_t y) = 0;
	virtual void putPixel(uint8_t color, uint8_t x, uint8_t y) = 0;
};