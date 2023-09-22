#include "OamSearchMode.hpp"
#include "HardwareRegisters.hpp"
#include "BitUtils.hpp"
#include "PPU.hpp"

OamSearchMode::OamSearchMode(MMU& mmu) :
	_mmu(mmu),
	_state(OamSearchMode::State::ReadY),
	_currentSprite(0)
{
	std::memset(_sprites, 0, sizeof(Sprite));
}

void OamSearchMode::start()
{
	_currentSprite = 0;
	tickAll();
}

void OamSearchMode::tick(size_t ticks)
{
	uint16_t spriteAddr = OamSearchMode::VRAM_OAM_ADDR + _currentSprite * 4;

	switch (_state)
	{
		case OamSearchMode::ReadY:
		{
			_sprites[_currentSprite].addr = spriteAddr;
			_sprites[_currentSprite].y = _mmu.read8(spriteAddr);
			_state = OamSearchMode::ReadX;
			break;
		}
		case OamSearchMode::ReadX:
		{
			_sprites[_currentSprite].x = _mmu.read8(spriteAddr + 1);
			uint8_t ly = _mmu.read8(HardwareRegisters::LY_ADDR);
			if (_currentSprite < MAX_SPRITE && _sprites[_currentSprite].y <= (ly + 16) && (ly + 16) < getSpriteHeight())
			{
				_currentSprite++;
			}
			_state = OamSearchMode::ReadY;
			break;
		}
		default:
			break;
	}
}

void OamSearchMode::tickAll()
{
	for (uint8_t i = 0; i < 40; i++)
	{
		uint16_t spriteAddr = OamSearchMode::VRAM_OAM_ADDR + i * 4;
		_sprites[i].addr = spriteAddr;
		_sprites[i].y = _mmu.read8(spriteAddr);
		_sprites[i].x = _mmu.read8(spriteAddr + 1);
		_sprites[i].tileId = _mmu.read8(spriteAddr + 2);
		_sprites[i].paletteId = BitUtils::GetBit(_mmu.read8(spriteAddr + 3), 4);
		_sprites[i].xFlip = BitUtils::GetBit(_mmu.read8(spriteAddr + 3), 5);
		_sprites[i].yFlip = BitUtils::GetBit(_mmu.read8(spriteAddr + 3), 6);
		_sprites[i].isBgAndWinOver = BitUtils::GetBit(_mmu.read8(spriteAddr + 3), 7);
		_currentSprite++;
	}
}

uint8_t OamSearchMode::getSpriteHeight() const
{
	return BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), PPU::LCDC_OBJ_SIZE_POS) ? 16 : 8;
}