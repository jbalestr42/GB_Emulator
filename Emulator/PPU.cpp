#include "PPU.hpp"

PPU::PPU(std::uint16_t width, std::uint16_t height, std::uint8_t pixelSize, std::uint8_t frameRate, const char* title) :
	_width(width),
	_height(height),
	_pixelSize(pixelSize),
	_frameRate(frameRate),
	_window(sf::VideoMode(_width * _pixelSize, _height * _pixelSize), title)
{
	_vertices.setPrimitiveType(sf::Quads);
	_vertices.resize(_width * _height * 4);

	for (std::uint16_t x = 0; x < _width; x++)
	{
		for (std::uint16_t y = 0; y < _height; y++)
		{
			sf::Vertex* quad = &_vertices[(x + y * _width) * 4];
			quad[0].position = sf::Vector2f(static_cast<float>(x * _pixelSize), static_cast<float>(y * _pixelSize));
			quad[1].position = sf::Vector2f(static_cast<float>((x + 1)) * _pixelSize, static_cast<float>(y * _pixelSize));
			quad[2].position = sf::Vector2f(static_cast<float>((x + 1)) * _pixelSize, static_cast<float>((y + 1) * _pixelSize));
			quad[3].position = sf::Vector2f(static_cast<float>(x * _pixelSize), static_cast<float>((y + 1) * _pixelSize));

			quad[0].color = sf::Color::Black;
			quad[1].color = sf::Color::Black;
			quad[2].color = sf::Color::Black;
			quad[3].color = sf::Color::Black;
		}
	}
	_window.setFramerateLimit(_frameRate);
}

void PPU::display()
{
	_window.draw(_vertices);
	_window.display();
}

void PPU::clear()
{
	for (std::uint16_t i = 0; i < _width * _height * 4; i++)
	{
		_vertices[i].color = sf::Color::Black;
	}
	_window.clear();
}

void PPU::close()
{
	_window.close();
}

bool PPU::isOpen()
{
	return _window.isOpen();
}

bool PPU::pollEvent(sf::Event& events)
{
	return _window.pollEvent(events);
}

/*void PPU::draw(uint16_t b1, uint16_t b2, uint16_t b3, CPU& cpu)
{
	cpu.setRegister(0xF, 0);

	for (uint16_t k = 0; k < b1; k++)
	{
		uint16_t line = cpu.getMemory(cpu.getIndexRegister() + k); // Get the line of the sprite to draw
		uint16_t y = cpu.getRegister(b2) % _height + k;  // Get the y position

		if (y < _height)
		{
			for (uint16_t j = 0, shift = 7; j < 8; j++, shift--)
			{
				uint16_t x = cpu.getRegister(b3) % _width + j; // Get the x position

				if (x < _width && (line & (0x1 << shift)) != 0) // Check if the bit must change
				{
					sf::Vertex* quad = &_vertices[(x + y * _width) * 4];
					// If pixel was activated we must turn it off and set register 0xF to 1
					if (quad[0].color == sf::Color::White)
					{
						quad[0].color = sf::Color::Black;
						quad[1].color = sf::Color::Black;
						quad[2].color = sf::Color::Black;
						quad[3].color = sf::Color::Black;
						cpu.setRegister(0xF, 1);
					}
					else
					{
						// If pixel was desactivated we activate it
						quad[0].color = sf::Color::White;
						quad[1].color = sf::Color::White;
						quad[2].color = sf::Color::White;
						quad[3].color = sf::Color::White;
					}
				}
			}
		}
	}
}*/