#include "PPU.hpp"
#include "HardwareRegisters.hpp"
#include "Interrupts.hpp"
#include "BitUtils.hpp"
#include "MMU.hpp"
#include "IDisplay.hpp"
#include <algorithm>

PPU::PPU(MMU& mmu, Interrupts& interrupts, IDisplay& display) :
	_mmu(mmu),
	_interrupts(interrupts),
	_display(display),
	_mode(PPU::Mode::OamSearch),
	_oamSearchMode(mmu),
	_ticks(0),
	_currentLine(0),
	_windowLineCounter(0),
	_lycInterruptRaiseDuringRendering(false)
{ }

void PPU::initialize()
{
	setMode(PPU::Mode::OamSearch);
	_oamSearchMode.start();
}

void PPU::tick()
{
	_ticks++;
	uint8_t ly = _mmu.read8(HardwareRegisters::LY_ADDR);
	uint8_t lyc = _mmu.read8(HardwareRegisters::LYC_ADDR);
	uint8_t stat = _mmu.read8(HardwareRegisters::STAT_ADDR);
	_mmu.write8(HardwareRegisters::STAT_ADDR, BitUtils::SetBit(stat, 2, ly == lyc));

	if (ly == lyc && isLYLYCInterruptEnabled() && !_lycInterruptRaiseDuringRendering)
	{
		_lycInterruptRaiseDuringRendering = true;
		_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
	}

	if (_mode == PPU::Mode::OamSearch && _ticks >= 80)
	{
		_ticks -= 80;
		setMode(PPU::Mode::PixelTransfer);
	}
	else if (_mode == PPU::Mode::PixelTransfer && _ticks >= 172)
	{
		if (isHBlankInterruptEnabled())
		{
			_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
		}
		_ticks -= 172;
		setMode(PPU::Mode::HBlank);
		draw();
	}
	else if (_mode == PPU::Mode::HBlank && _ticks >= 204)
	{
		_currentLine++;
		_lycInterruptRaiseDuringRendering = false;
		_ticks -= 204;
		if (_currentLine == 144)
		{
			if (isVBlankInterruptEnabled())
			{
				_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
			}
			_interrupts.raiseInterrupt(Interrupts::Type::VBlank);
			setMode(PPU::Mode::VBlank);
			_display.display();
		}
		else
		{
			if (isOamInterruptEnabled())
			{
				_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
			}
			_oamSearchMode.start();
			setMode(PPU::Mode::OamSearch);
		}
	}
	else if (_mode == PPU::Mode::VBlank && _ticks >= 456)
	{
		_currentLine++;
		_ticks -= 456;
		if (_currentLine > 153)
		{
			if (isOamInterruptEnabled())
			{
				_interrupts.raiseInterrupt(Interrupts::Type::LCDStat);
			}
			_oamSearchMode.start();
			setMode(PPU::Mode::OamSearch);
			_currentLine = 0;
			_windowLineCounter = 0;
		}
		_lycInterruptRaiseDuringRendering = false;
	}
	_mmu.write8(HardwareRegisters::LY_ADDR, _currentLine);
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

		if (BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_OBJ_ENABLE_POS))
		{
			drawSprites();
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

	for (int x = 0; x < PPU::WIDTH; ++x)
	{
		int xOffset = ((scrollX + x) / PPU::TILE_WIDTH_PX) % PPU::TILEMAP_WIDTH_TILE;
		int xTileOffset = (scrollX + x) % PPU::TILE_WIDTH_PX;

		uint16_t tileAddr = getTileAddr(tileMapAddr + yOffset + xOffset);
		uint8_t colorId = getColorId(tileAddr, xTileOffset, yTileOffset);
		uint8_t color = getColor(colorId, HardwareRegisters::BGP_ADDR);

		_display.putPixel(color, x, _currentLine);
	}
}

void PPU::drawWindow()
{
	uint8_t winX = _mmu.read8(HardwareRegisters::WX_ADDR) - 7;
	uint8_t winY = _mmu.read8(HardwareRegisters::WY_ADDR);

	if (winY > _currentLine || winX > PPU::WIDTH)
	{
		return;
	}

	uint16_t tileMapAddr = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_WIN_TILEMAP_POS) ? VRAM_TILEMAP_2_ADDR : VRAM_TILEMAP_1_ADDR;
	int yOffset = ((_windowLineCounter / PPU::TILE_HEIGHT_PX) % PPU::TILEMAP_HEIGHT_TILE) * PPU::TILEMAP_WIDTH_TILE;
	int yTileOffset = _windowLineCounter % PPU::TILE_HEIGHT_PX;

	for (int x = winX; x < PPU::WIDTH; ++x)
	{
		int xOffset = (x - winX) / PPU::TILE_WIDTH_PX;
		int xTileOffset = (x - winX) % PPU::TILE_WIDTH_PX;

		uint16_t tileAddr = getTileAddr(tileMapAddr + yOffset + xOffset);
		uint8_t colorId = getColorId(tileAddr, xTileOffset, yTileOffset);
		uint8_t color = getColor(colorId, HardwareRegisters::BGP_ADDR);

		_display.putPixel(color, x, _currentLine);
	}
	_windowLineCounter++;
}

void PPU::drawSprites()
{
	uint16_t tileMapAddr = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_WIN_TILEMAP_POS) ? VRAM_TILEMAP_2_ADDR : VRAM_TILEMAP_1_ADDR;

	std::vector<OamSearchMode::Sprite> renderedSprite;
	OamSearchMode::Sprite* sprites = _oamSearchMode.getSprites();
	for (int i = 0; i < _oamSearchMode.getSpriteCount() && renderedSprite.size() < 10; ++i)
	{
		OamSearchMode::Sprite& sprite = sprites[i];
		uint8_t spriteSize = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_OBJ_SIZE_POS) ? 16 : 8;

		if (sprite.y <= (_currentLine + 16) && (_currentLine + 16) < sprite.y + spriteSize)
		{
			if ((sprite.x - 8 + PPU::TILE_WIDTH_PX) < 0 || (sprite.x - 8) >= PPU::WIDTH)
			{
				continue;
			}

			renderedSprite.push_back(sprite);
		}
	}

	std::sort(renderedSprite.begin(), renderedSprite.end(),
		[](OamSearchMode::Sprite& s1, OamSearchMode::Sprite& s2) { return s1.x == s2.x ? s1.tileId < s2.tileId : s1.x > s2.x; });

	for (size_t i = 0; i < renderedSprite.size(); ++i)
	{
		OamSearchMode::Sprite& sprite = renderedSprite[i];
		uint8_t tileId = sprite.tileId;
		uint8_t spriteSize = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_OBJ_SIZE_POS) ? 16 : 8;
		bool isStacked = spriteSize > PPU::TILE_HEIGHT_PX;
		if (isStacked)
		{
			tileId &= 0xFE; // Ignore bit 0 for 8x16 sprite
		}

		int yTileCoord = sprite.yFlip ? spriteSize - 1 - _currentLine + sprite.y - 16 : _currentLine - sprite.y + 16;

		for (int xOffset = 0; xOffset < 8; xOffset++)
		{
			int xScreenCoord = sprite.x - 8 + xOffset;

			if (xScreenCoord < 0)
			{
				continue;
			}
			if (xScreenCoord > PPU::WIDTH)
			{
				break;
			}

			int xTileCoord = sprite.xFlip ? PPU::TILE_WIDTH_PX - 1 - xOffset : xOffset;

			uint16_t tileAddr = PPU::VRAM_TILEDATA_0_ADDR + PPU::BYTES_PER_TILE * tileId;
			uint8_t colorId = getColorId(tileAddr, xTileCoord, yTileCoord);
			uint16_t paletteAddr = sprite.paletteId ? HardwareRegisters::OBP1_ADDR : HardwareRegisters::OBP0_ADDR;
			uint8_t color = getColor(colorId, paletteAddr);

			bool isPixelVisible = colorId != 0; // 0 is transparent
			bool shouldRenderPixel = !sprite.isBgAndWinOver || _display.isPixelWhite(xScreenCoord, _currentLine);
			if (isPixelVisible && shouldRenderPixel)
			{
				_display.putPixel(color, xScreenCoord, _currentLine);
			}
		}
	}
}

void PPU::setMode(PPU::Mode mode)
{
	_mode = mode;
	uint8_t stat = _mmu.read8(HardwareRegisters::STAT_ADDR) & 0b11111100;
	_mmu.write8(HardwareRegisters::STAT_ADDR, stat | static_cast<uint8_t>(_mode));
}

uint16_t PPU::getTileAddr(uint16_t tileMapAddr) const
{
	uint8_t tileId = _mmu.read8(tileMapAddr);
	bool isSigned = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), LCDC_BG_WIN_DATA_POS);
	uint16_t tileSetAddr = isSigned ? PPU::VRAM_TILEDATA_0_ADDR : PPU::VRAM_TILEDATA_2_ADDR;
	int finalTileId = isSigned ? tileId : static_cast<int8_t>(tileId);
	return tileSetAddr + PPU::BYTES_PER_TILE * finalTileId;
}

uint8_t PPU::getColorId(uint16_t tileAddr, uint8_t xTileCoord, uint8_t yTileCoord) const
{
	uint8_t lsb = BitUtils::GetBit(_mmu.read8(tileAddr + yTileCoord * 2), 7 - xTileCoord);
	uint8_t msb = BitUtils::GetBit(_mmu.read8(tileAddr + yTileCoord * 2 + 1), 7 - xTileCoord);
	return (msb << 1) | lsb;
}

uint8_t PPU::getColor(uint8_t colorId, uint16_t paletteAddr) const
{
	uint8_t palette = _mmu.read8(paletteAddr);
	return (palette >> (colorId * 2)) & 0x3;
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