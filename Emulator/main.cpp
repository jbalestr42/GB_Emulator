#include <SFML/Graphics.hpp>
#include <iostream>
#include "CPU.hpp"
#include "PPU.hpp"
#include "MMU.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "HardwareRegisters.hpp"

int main(int argc, char* argv[])
{
    MMU mmu;
    Interrupts interrupts(mmu);
    CPU cpu(mmu, interrupts);
    Timer timer(interrupts);
    PPU ppu(mmu, interrupts, 160, 144, 4, 60, "GAME BOY");
    Input input(mmu);

    mmu.addMemoryOverride(HardwareRegisters::DIV_ADDR, MMU::MemoryOverride(
        [&timer]() { return static_cast<uint8_t>(timer.getDiv()); },
        [&timer](uint8_t value) { timer.setDiv(); }));
    mmu.addMemoryOverride(HardwareRegisters::TIMA_ADDR, MMU::MemoryOverride(
        [&timer]() { return static_cast<uint8_t>(timer.getTima()); },
        [&timer](uint8_t value) { timer.setTima(value); }));
    mmu.addMemoryOverride(HardwareRegisters::TMA_ADDR, MMU::MemoryOverride(
        [&timer]() { return timer.getTma(); },
        [&timer](uint8_t value) { timer.setTma(value); }));
    mmu.addMemoryOverride(HardwareRegisters::TAC_ADDR, MMU::MemoryOverride(
        [&timer]() { return timer.getTac(); },
        [&timer](uint8_t value) { timer.setTac(value); }));
    mmu.addMemoryOverride(HardwareRegisters::DMA_ADDR, MMU::MemoryOverride(
        nullptr,
        [&mmu](uint8_t value) {
            uint16_t sourceAddr = value << 8;
            for (uint16_t i = 0; i < 160; ++i)
            {
                mmu.write8(0xFE00 + i, mmu.read8(sourceAddr + i));
            }
        }));
    mmu.addMemoryOverride(HardwareRegisters::JOYP_ADDR, MMU::MemoryOverride(
        [&input]() { return input.read8(); },
        [&input](uint8_t value) { input.write8(value); }));

    // Initialize hardware registers when no boot rom is used
    mmu.write8(HardwareRegisters::JOYP_ADDR, 0xCF);
    mmu.write8(HardwareRegisters::SB_ADDR, 0x00);
    mmu.write8(HardwareRegisters::SC_ADDR, 0x7E);
    mmu.write8(HardwareRegisters::DIV_ADDR, 0xAB);
    mmu.write8(HardwareRegisters::TIMA_ADDR, 0x00);
    mmu.write8(HardwareRegisters::TMA_ADDR, 0x00);
    mmu.write8(HardwareRegisters::TAC_ADDR, 0xF8);
    mmu.write8(HardwareRegisters::IF_ADDR, 0xE1);
    mmu.write8(HardwareRegisters::LCDC_ADDR, 0x91);
    mmu.write8(HardwareRegisters::STAT_ADDR, 0x85);
    mmu.write8(HardwareRegisters::SCY_ADDR, 0x00);
    mmu.write8(HardwareRegisters::SCX_ADDR, 0x00);
    mmu.write8(HardwareRegisters::LY_ADDR, 0x91);
    mmu.write8(HardwareRegisters::LYC_ADDR, 0x00);
    mmu.write8(HardwareRegisters::DMA_ADDR, 0xFF);
    mmu.write8(HardwareRegisters::BGP_ADDR, 0xFC);
    mmu.write8(HardwareRegisters::WY_ADDR, 0x00);
    mmu.write8(HardwareRegisters::WX_ADDR, 0x00);
    mmu.write8(HardwareRegisters::IE_ADDR, 0x00);
    
    mmu.loadRom(argv[1]);

    sf::Clock clock;
    sf::Time time;
    sf::Time frameDuration = sf::seconds(1.0f / 60.0f);
    while (ppu.isOpen())
    {
        sf::Event event;
        while (ppu.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
				ppu.close();
            }
        }

        while (time < frameDuration)
        {
            time += clock.restart();

            size_t ticks = cpu.update();
            timer.update(ticks);
            ppu.update(ticks);

        }
        time -= frameDuration;
        input.update();
    }

    return 0;
}