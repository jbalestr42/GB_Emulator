#include "PixelFetcher.hpp"
#include "HardwareRegisters.hpp"
#include "BitUtils.hpp"
#include "Fifo.hpp"
#include "PPU.hpp"

PixelFetcher::PixelFetcher(MMU& mmu, Fifo& fifo) :
	_mmu(mmu),
	_fifo(fifo),
	_state(PixelFetcher::State::GetTileId),
	_ticks(0),
	_memoryAddr(0x0),
	_xOffset(0),
	_y(0),
	_tileId(0),
	_dataLow(0),
	_dataHigh(0)
{

}

void PixelFetcher::start(uint16_t memoryAddr, uint8_t xOffset, uint8_t y)
{
	_memoryAddr = memoryAddr;
	_xOffset = xOffset;
	_y = y;
	_ticks = 0;
}

void PixelFetcher::tick(size_t ticks)
{
	// Each state is 2 ticks, we can skip 1 tick every tick
	_ticks++;
	if (ticks % 2 != 0)
	{
		switch (_state)
		{
		case PixelFetcher::GetTileId:
			_tileId = _mmu.read8(_memoryAddr + _xOffset);
			_state = PixelFetcher::State::GetTileDataLow;
			break;
		case PixelFetcher::GetTileDataLow:
		{
			bool isSigned = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), PPU::LCDC_BG_WIN_DATA_POS);
			uint16_t tileMapAddr = isSigned ? PPU::VRAM_TILEDATA_0_ADDR : PPU::VRAM_TILEDATA_2_ADDR;
			_dataLow = _mmu.read8(tileMapAddr + (isSigned ? static_cast<int8_t>(_tileId) * 8 : _tileId * 8));
			_state = PixelFetcher::State::GetTileDataHigh;
			break;
		}
		case PixelFetcher::GetTileDataHigh:
		{
			bool isSigned = BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), PPU::LCDC_BG_WIN_DATA_POS);
			uint16_t tileMapAddr = isSigned ? PPU::VRAM_TILEDATA_0_ADDR : PPU::VRAM_TILEDATA_2_ADDR;
			_dataHigh = _mmu.read8(tileMapAddr + 1 + (isSigned ? static_cast<int8_t>(_tileId) * 8 : _tileId * 8));
			_state = PixelFetcher::State::Push;
			break;
		}
		case PixelFetcher::Push:
			// If fifo is not empty we keep trying
			if (_fifo.isEmpty())
			{
				for (size_t i = 0; i < 8; i++)
				{
					uint8_t color = _dataLow & (1 << i) | ((_dataHigh & (1 << i)) << 1);
					_fifo.push(Fifo::Pixel(color));
				}
			}
			_state = PixelFetcher::State::GetTileId;
			break;
		default:
			break;
		}
	}
}