#include "OamSearchMode.hpp"
#include "BitUtils.hpp"
#include "MMU.hpp"

OamSearchMode::OamSearchMode(MMU& mmu) :
	_mmu(mmu),
	_currentSprite(0)
{
	std::memset(_sprites, 0, sizeof(Sprite));
}

void OamSearchMode::start()
{
	_currentSprite = 0;
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