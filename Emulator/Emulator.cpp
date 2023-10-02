#include "Emulator.hpp"
#include "HardwareRegisters.hpp"

Emulator::Emulator() :
	_mmu(),
	_interrupts(_mmu),
	_cpu(_mmu, _interrupts),
	_timer(_interrupts),
	_ppu(_mmu, _interrupts, 160, 144, 4, 60, "Game Boy"),
	_input(_mmu)
{ }

void Emulator::initialize()
{
    _mmu.initialize();
    _cpu.initialize();
    _ppu.initialize();

    // Initialize special access to memory
    _mmu.addMemoryOverride(HardwareRegisters::DIV_ADDR, MMU::MemoryOverride(
        [&]() { return _timer.getDiv(); },
        [&](uint8_t value) { _timer.setDiv(); }));
    _mmu.addMemoryOverride(HardwareRegisters::TIMA_ADDR, MMU::MemoryOverride(
        [&]() { return _timer.getTima(); },
        [&](uint8_t value) { _timer.setTima(value); }));
    _mmu.addMemoryOverride(HardwareRegisters::TMA_ADDR, MMU::MemoryOverride(
        [&]() { return _timer.getTma(); },
        [&](uint8_t value) { _timer.setTma(value); }));
    _mmu.addMemoryOverride(HardwareRegisters::TAC_ADDR, MMU::MemoryOverride(
        [&]() { return _timer.getTac(); },
        [&](uint8_t value) { _timer.setTac(value); }));
    _mmu.addMemoryOverride(HardwareRegisters::DMA_ADDR, MMU::MemoryOverride(
        nullptr,
        [&](uint8_t value) {
            uint16_t sourceAddr = value << 8;
            for (uint16_t i = 0; i < 160; ++i)
            {
                _mmu.write8(0xFE00 + i, _mmu.read8(sourceAddr + i));
            }
        }));
    _mmu.addMemoryOverride(HardwareRegisters::JOYP_ADDR, MMU::MemoryOverride(
        [&]() { return _input.read8(); },
        [&](uint8_t value) { _input.write8(value); }));
    _mmu.addMemoryOverride(HardwareRegisters::BOOT_ADDR, MMU::MemoryOverride(
        [&]() { return _mmu.getCartridge().getBoot(); },
        [&](uint8_t value) { _mmu.getCartridge().setBoot(); }));
}

void Emulator::update()
{
    // TODO separate display from ppu
    sf::Clock clock;
    sf::Time time;
    sf::Time frameDuration = sf::seconds(1.0f / 60.0f);
    while (_ppu.isOpen())
    {
        sf::Event event;
        while (_ppu.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                _ppu.close();
            }
        }

        while (time < frameDuration)
        {
            time += clock.restart();

            _cpu.tick();
            _timer.tick();
            _ppu.tick();

        }
        _input.tick();
        time -= frameDuration;
    }
}

bool Emulator::loadRom(const char* path)
{
    return _mmu.loadRom(path);
}

void Emulator::enableBootRom(bool isEnable)
{
    if (!isEnable)
    {
        // Initialize hardware registers
        _mmu.write8(HardwareRegisters::JOYP_ADDR, 0xCF);
        _mmu.write8(HardwareRegisters::SB_ADDR, 0x00);
        _mmu.write8(HardwareRegisters::SC_ADDR, 0x7E);
        _mmu.write8(HardwareRegisters::DIV_ADDR, 0xAB);
        _mmu.write8(HardwareRegisters::TIMA_ADDR, 0x00);
        _mmu.write8(HardwareRegisters::TMA_ADDR, 0x00);
        _mmu.write8(HardwareRegisters::TAC_ADDR, 0xF8);
        _mmu.write8(HardwareRegisters::IF_ADDR, 0xE1);
        _mmu.write8(HardwareRegisters::LCDC_ADDR, 0x91);
        _mmu.write8(HardwareRegisters::STAT_ADDR, 0x85);
        _mmu.write8(HardwareRegisters::SCY_ADDR, 0x00);
        _mmu.write8(HardwareRegisters::SCX_ADDR, 0x00);
        _mmu.write8(HardwareRegisters::LY_ADDR, 0x91);
        _mmu.write8(HardwareRegisters::LYC_ADDR, 0x00);
        _mmu.write8(HardwareRegisters::DMA_ADDR, 0xFF);
        _mmu.write8(HardwareRegisters::BGP_ADDR, 0xFC);
        _mmu.write8(HardwareRegisters::WY_ADDR, 0x00);
        _mmu.write8(HardwareRegisters::WX_ADDR, 0x00);
        _mmu.write8(HardwareRegisters::IE_ADDR, 0x00);

        // Initialize cpu registers
        Registers& registers = _cpu.getRegisters();
        registers.a = 0x01;
        registers.c = 0x13;
        registers.e = 0xD8;
        registers.h = 0x01;
        registers.l = 0x4D;
        registers.pc = CPU::START_PC_ADDR;
        registers.flags.z = 1;
        registers.flags.n = 0;
        registers.flags.h = 1;
        registers.flags.c = 1;
        registers.af();
    }

    // set cartridge
    _mmu.getCartridge().enableBoot(isEnable);
}