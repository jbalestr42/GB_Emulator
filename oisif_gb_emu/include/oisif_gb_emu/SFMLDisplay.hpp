#pragma once

#include <SFML\Graphics.hpp>
#include <cstdint>
#include "IDisplay.hpp"

class SFMLDisplay : public IDisplay
{
public:
	SFMLDisplay(uint16_t width, uint16_t height, uint8_t pixelSize, uint8_t frameRate, const char* title);
	~SFMLDisplay() = default;

	void display() override;
	void clear() override;
	void close() override;
	bool isOpen() override;
	void pollEvent() override;
	bool isPixelWhite(uint8_t x, uint8_t y) override;
	void putPixel(uint8_t color, uint8_t x, uint8_t y) override;

	static const sf::Color LIGHTEST_GREEN;
	static const sf::Color LIGHT_GREEN;
	static const sf::Color DARK_GREEN;
	static const sf::Color DARKEST_GREEN;

private:
	static const sf::Color COLORS[];

	sf::RenderWindow _window;
	sf::VertexArray _vertices;

	uint16_t _width;
	uint16_t _height;
	uint8_t _pixelSize;
	uint8_t _frameRate;
};