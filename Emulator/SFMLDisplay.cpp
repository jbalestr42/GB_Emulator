#include "SFMLDisplay.hpp"

const sf::Color SFMLDisplay::COLOR_MAPPING[] = {	sf::Color(255, 255, 255),
													sf::Color(170, 170, 170),
													sf::Color(85, 85, 85),
													sf::Color(0, 0, 0) };


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

			quad[0].color = sf::Color::White;
			quad[1].color = sf::Color::White;
			quad[2].color = sf::Color::White;
			quad[3].color = sf::Color::White;
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
	return _vertices[(x + y * _width) * 4].color == sf::Color::White;
}

void SFMLDisplay::putPixel(uint8_t color, uint8_t x, uint8_t y)
{
	sf::Vertex* quad = &_vertices[(x + y * _width) * 4];

	sf::Color c = COLOR_MAPPING[color];
	quad[0].color = c;
	quad[1].color = c;
	quad[2].color = c;
	quad[3].color = c;
}