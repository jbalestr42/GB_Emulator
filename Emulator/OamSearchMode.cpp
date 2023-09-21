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

uint8_t OamSearchMode::getSpriteHeight() const
{
	return BitUtils::GetBit(_mmu.read8(HardwareRegisters::LCDC_ADDR), PPU::LCDC_OBJ_SIZE_POS) ? 16 : 8;
}