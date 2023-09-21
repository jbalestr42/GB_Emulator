#include "PixelTransferMode.hpp"
#include "HardwareRegisters.hpp"
#include "BitUtils.hpp"
#include "PPU.hpp"

PixelTransferMode::PixelTransferMode(MMU& mmu, PPU& ppu) :
	_mmu(mmu),
	_ppu(ppu),
	_pixelFetcher(_mmu, _fifo),
	_x(0),
	_isFetchingWindow(false)
{ }

void PixelTransferMode::start()
{
	_x = 0;
	_isFetchingWindow = false;
	startBackgroundFetch();
}

void PixelTransferMode::tick(size_t ticks)
{
	_pixelFetcher.tick(1);

	uint8_t ly = _mmu.read8(HardwareRegisters::LY_ADDR);
	if (getLcdcValue(PPU::LCDC_BG_WIN_PRIORITY_ENABLE_POS))
	{
		// if () < 8 pixe in fetcher
		// return
		
		/*
		if (!_isFetchingWindow
			&& getLcdcValue(PPU::LCDC_WIN_ENABLE_POS)
			&& ly >= _mmu.read8(HardwareRegisters::WY_ADDR)
			&& _x == (_mmu.read8(HardwareRegisters::WX_ADDR) - 7))
		{
			_isFetchingWindow = false;
			startWindowFetch();
		}*/
	}
	if (_fifo.size() >= 8)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			_ppu.putPixel(_fifo.pop().color, _x + i, ly);
		}
		_x++;
	}
}

void PixelTransferMode::startBackgroundFetch()
{
	uint16_t addr = getLcdcValue(PPU::LCDC_BG_TILEMAP_POS) ? PPU::VRAM_TILEMAP_2_ADDR : PPU::VRAM_TILEMAP_1_ADDR;
	uint8_t scx = _mmu.read8(HardwareRegisters::SCX_ADDR);
	uint8_t scy = _mmu.read8(HardwareRegisters::SCY_ADDR);
	uint8_t ly = _mmu.read8(HardwareRegisters::LY_ADDR);
	uint16_t offset = 32 * ((ly + scy) & 0xFF) / 8;

	_pixelFetcher.start(addr + offset, scx / 8, (ly + scy) & 0xFF);
}

void PixelTransferMode::startWindowFetch()
{
	uint16_t addr = getLcdcValue(PPU::LCDC_WIN_TILEMAP_POS) ? PPU::VRAM_TILEMAP_2_ADDR : PPU::VRAM_TILEMAP_1_ADDR;
	uint8_t wx = _mmu.read8(HardwareRegisters::WX_ADDR);
	uint8_t wy = _mmu.read8(HardwareRegisters::WY_ADDR);
	uint8_t ly = _mmu.read8(HardwareRegisters::LY_ADDR);
	uint16_t offset = 32 * ((ly - wy) / 8);

	_pixelFetcher.start(addr + offset, (_x - wx + 7) / 8, (ly - wy) % 8);
}

bool PixelTransferMode::getLcdcValue(size_t pos)
{
	return BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), pos);
}