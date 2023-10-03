#include "SFMLDisplay.hpp"

const sf::Color SFMLDisplay::LIGHTEST_GREEN = sf::Color(155, 188, 15);
const sf::Color SFMLDisplay::LIGHT_GREEN = sf::Color(139, 172, 15);
const sf::Color SFMLDisplay::DARK_GREEN = sf::Color(48, 98, 48);
const sf::Color SFMLDisplay::DARKEST_GREEN = sf::Color(15, 56, 15);

const sf::Color SFMLDisplay::COLORS[] = {	LIGHTEST_GREEN,
											LIGHT_GREEN,
											DARK_GREEN,
											DARKEST_GREEN };


SFMLDisplay::SFMLDisplay(uint16_t width, uint16_t height, uint8_t pixelSize, uint8_t frameRate, const char* title) :
	_window(sf::VideoMode(width * pixelSize, height * pixelSize), title),
	_width(width),
	_height(height),
	_pixelSize(pixelSize),
	_frameRate(frameRate)
{
	_vertices.setPrimitiveType(sf::Quads);
	_vertices.resize(_width * _height * 4);

	for (std::uint16_t x = 0; x < _width; x++)
	{
		for (std::uint16_t y = 0; y < _height; y++)
		{
			sf::Vertex* quad = &_vertices[(x + y * _width) * 4];
			quad[0].position = sf::Vector2f(static_cast<float>(x * _pixelSize), static_cast<float>(y * _pixelSize));
			quad[1].position = sf::Vector2f(static_cast<float>((x + 1) * _pixelSize), static_cast<float>(y * _pixelSize));
			quad[2].position = sf::Vector2f(static_cast<float>((x + 1) * _pixelSize), static_cast<float>((y + 1) * _pixelSize));
			quad[3].position = sf::Vector2f(static_cast<float>(x * _pixelSize), static_cast<float>((y + 1) * _pixelSize));

			quad[0].color = SFMLDisplay::LIGHTEST_GREEN;
			quad[1].color = SFMLDisplay::LIGHTEST_GREEN;
			quad[2].color = SFMLDisplay::LIGHTEST_GREEN;
			quad[3].color = SFMLDisplay::LIGHTEST_GREEN;
		}
	}
	_window.setFramerateLimit(_frameRate);
}

void SFMLDisplay::display()
{
	_window.draw(_vertices);
	_window.display();
}

void SFMLDisplay::clear()
{
	for (std::uint16_t i = 0; i < _width * _height * 4; i++)
	{
		_vertices[i].color = sf::Color::Black;
	}
	_window.clear();
}

void SFMLDisplay::close()
{
	_window.close();
}

bool SFMLDisplay::isOpen()
{
	return _window.isOpen();
}

void SFMLDisplay::pollEvent()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_window.close();
		}
	}
}

bool SFMLDisplay::isPixelWhite(uint8_t x, uint8_t y)
{
	return _vertices[(x + y * _width) * 4].color == SFMLDisplay::LIGHTEST_GREEN;
}

void SFMLDisplay::putPixel(uint8_t color, uint8_t x, uint8_t y)
{
	sf::Vertex* quad = &_vertices[(x + y * _width) * 4];

	sf::Color c = COLORS[color];
	quad[0].color = c;
	quad[1].color = c;
	quad[2].color = c;
	quad[3].color = c;
}