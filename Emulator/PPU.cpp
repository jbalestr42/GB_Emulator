#include "PPU.hpp"
#include "HardwareRegisters.hpp"
#include "HBlankMode.hpp"
#include "VBlankMode.hpp"
#include "PixelTransferMode.hpp"
#include "OamSearchMode.hpp"

PPU::PPU(MMU& mmu, Interrupts& interrupts, std::uint16_t width, std::uint16_t height, std::uint8_t pixelSize, std::uint8_t frameRate, const char* title) :
	_mmu(mmu),
	_interrupts(interrupts),
	_mode(PPU::Mode::OamSearch),
	_oamSearchMode(mmu),
	_pixelTransferMode(mmu, *this),
	_ticks(0),
	_currentLine(0),
	_windowLineCounter(0),
	_lycInterruptRaiseDuringRendering(false),
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

	setMode(PPU::Mode::OamSearch);
	_oamSearchMode.start();
}

void PPU::update(size_t ticks)
{
	//for (size_t i = 0; i < ticks; i++)
	{
		//_ticks++;
		_ticks += ticks;
		uint8_t ly = _mmu.read8(HardwareRegisters::LY_ADDR);
		uint8_t lyc = _mmu.read8(HardwareRegisters::LYC_ADDR);
		uint8_t stat = _mmu.read8(HardwareRegisters::STAT_ADDR);
		_mmu.write8(HardwareRegisters::STAT_ADDR, BitUtils::SetBit(stat, 6, ly == lyc));

		if (ly == lyc && isLYLYCInterruptEnabled() && !_lycInterruptRaiseDuringRendering)
		{
			_lycInterruptRaiseDuringRendering = true;
			_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
		}

		if (_mode == PPU::Mode::OamSearch)
		{
			//_oamSearchMode.tick(1);

			if (_ticks >= _oamSearchMode.getMaxTick())
			{
				setMode(PPU::Mode::PixelTransfer);
				//_pixelTransferMode.start();
			}
		}
		else if (_mode == PPU::Mode::PixelTransfer)
		{
			//_pixelTransferMode.tick(1);
			if (_ticks >= _pixelTransferMode.getMaxTick())
			{
				if (isHBlankInterruptEnabled())
				{
					_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
				}
				setMode(PPU::Mode::HBlank);
				draw();
			}
		}
		else if (_mode == PPU::Mode::HBlank)
		{
			//_hBlank.tick(1);
			if (_ticks >= _hBlank.getMaxTick())
			{
				_lycInterruptRaiseDuringRendering = false;
				_currentLine++;
				ly++;
				_mmu.write8(HardwareRegisters::LY_ADDR, ly);
				if (_currentLine == 144)//ly == 144)
				{
					if (isVBlankInterruptEnabled())
					{
						_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
					}
					_interrupts.raiseInterrupt(Interrupts::Type::VBlank);
					setMode(PPU::Mode::VBlank);
					display();
				}
				else
				{
					if (isOamInterruptEnabled())
					{
						_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
					}
					//_oamSearchMode.start();
					setMode(PPU::Mode::OamSearch);
				}
			}
		}
		else if (_mode == PPU::Mode::VBlank)
		{
			//_vBlank.tick(1);
			if (_ticks >= _vBlank.getMaxTick())
			{
				ly++;
				_currentLine++;
				_lycInterruptRaiseDuringRendering = false;
				_mmu.write8(HardwareRegisters::LY_ADDR, ly);
				if (_currentLine > 153)// ly == 1)
				{
					if (isOamInterruptEnabled())
					{
						_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
					}
					_mmu.write8(HardwareRegisters::LY_ADDR, 0);
					//_oamSearchMode.start();
					_currentLine = 0;
					_windowLineCounter = 0;
					setMode(PPU::Mode::OamSearch);
				}
				else
				{
					setMode(PPU::Mode::VBlank);
				}
			}
		}
	}
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

void PPU::draw()
{
	if (BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_LCD_PPU_ENABLE_POS))
	{
		if (BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_BG_WIN_PRIORITY_ENABLE_POS))
		{
			drawBackground();

			if (BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_WIN_ENABLE_POS))
			{
				drawWindow();
			}
		}
	}
}

void PPU::drawBackground()
{
	uint8_t scrollX = _mmu.read8(HardwareRegisters::SCX_ADDR);
	uint8_t scrollY = _mmu.read8(HardwareRegisters::SCY_ADDR);

	uint16_t tileMapAddr = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_BG_TILEMAP_POS) ? VRAM_TILEMAP_2_ADDR : VRAM_TILEMAP_1_ADDR;
	int yOffset = (((scrollY + _currentLine) / PPU::TILE_HEIGHT_PX) % PPU::TILEMAP_HEIGHT_TILE) * PPU::TILEMAP_WIDTH_TILE;
	int yTileOffset = (scrollY + _currentLine) % PPU::TILE_HEIGHT_PX;

	for (int x = 0; x < _width; ++x)
	{
		int xOffset = ((scrollX + x) / PPU::TILE_WIDTH_PX) % PPU::TILEMAP_WIDTH_TILE;
		int xTileOffset = (scrollX + x) % PPU::TILE_WIDTH_PX;

		uint8_t tileId = _mmu.read8(tileMapAddr + yOffset + xOffset);
		bool isSigned = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_BG_WIN_DATA_POS);
		uint16_t tileSetAddr = isSigned ? PPU::VRAM_TILEDATA_0_ADDR : PPU::VRAM_TILEDATA_2_ADDR;
		int finalTileId = isSigned ? tileId : static_cast<int8_t>(tileId);

		uint16_t tileAddr = tileSetAddr + PPU::BYTES_PER_TILE * finalTileId;
		uint8_t lsb = BitUtils::GetBit(_mmu.read8(tileAddr + yTileOffset * 2), 7 - xTileOffset);
		uint8_t msb = BitUtils::GetBit(_mmu.read8(tileAddr + yTileOffset * 2 + 1), 7 - xTileOffset);
		uint8_t colorValue = (msb << 1) | lsb;
		putPixel(colorValue, x, _currentLine);
	}
}

void PPU::drawWindow()
{
	uint8_t winX = _mmu.read8(HardwareRegisters::WX_ADDR) - 7;
	uint8_t winY = _mmu.read8(HardwareRegisters::WY_ADDR);

	if (winY > _currentLine || winX > _width)
	{
		return;
	}

	uint16_t tileMapAddr = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_WIN_TILEMAP_POS) ? VRAM_TILEMAP_2_ADDR : VRAM_TILEMAP_1_ADDR;
	int yOffset = ((_windowLineCounter / PPU::TILE_HEIGHT_PX) % PPU::TILEMAP_HEIGHT_TILE) * PPU::TILEMAP_WIDTH_TILE;
	int yTileOffset = _windowLineCounter % PPU::TILE_HEIGHT_PX;

	for (int x = winX; x < _width; ++x)
	{
		int xOffset = (x - winX) / PPU::TILE_WIDTH_PX;
		int xTileOffset = (x - winX) % PPU::TILE_WIDTH_PX;

		uint8_t tileId = _mmu.read8(tileMapAddr + yOffset + xOffset);
		bool isSigned = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_BG_WIN_DATA_POS);
		uint16_t tileSetAddr = isSigned ? PPU::VRAM_TILEDATA_0_ADDR : PPU::VRAM_TILEDATA_2_ADDR;
		int finalTileId = isSigned ? tileId : static_cast<int8_t>(tileId);

		uint16_t tileAddr = tileSetAddr + PPU::BYTES_PER_TILE * finalTileId;
		uint8_t lsb = BitUtils::GetBit(_mmu.read8(tileAddr + yTileOffset * 2), 7 - xTileOffset);
		uint8_t msb = BitUtils::GetBit(_mmu.read8(tileAddr + yTileOffset * 2 + 1), 7 - xTileOffset);
		uint8_t colorValue = (msb << 1) | lsb;
		putPixel(colorValue, x, _currentLine);
	}
	_windowLineCounter++;
}

void PPU::setMode(PPU::Mode mode)
{
	_mode = mode;
	_ticks = 0;

	uint8_t stat = _mmu.read8(HardwareRegisters::STAT_ADDR) & 0b11111100;
	_mmu.write8(HardwareRegisters::STAT_ADDR, stat | static_cast<uint8_t>(_mode));
}

void PPU::putPixel(uint8_t color, uint8_t x, uint8_t y)
{
	static sf::Color palette[4] = { sf::Color(255, 255, 255), sf::Color(128, 128, 128), sf::Color(64, 64, 64), sf::Color(0, 0, 0) };
	sf::Vertex* quad = &_vertices[(x + y * _width) * 4];

	quad[0].color = palette[color];
	quad[1].color = palette[color];
	quad[2].color = palette[color];
	quad[3].color = palette[color];
}

bool PPU::isLYLYCInterruptEnabled() const
{
	return BitUtils::GetBit(_mmu.read8(HardwareRegisters::STAT_ADDR), 6);
}

bool PPU::isHBlankInterruptEnabled() const
{
	return BitUtils::GetBit(_mmu.read8(HardwareRegisters::STAT_ADDR), 3);
}

bool PPU::isVBlankInterruptEnabled() const
{
	return BitUtils::GetBit(_mmu.read8(HardwareRegisters::STAT_ADDR), 4);
}

bool PPU::isOamInterruptEnabled() const
{
	return BitUtils::GetBit(_mmu.read8(HardwareRegisters::STAT_ADDR), 5);
}
