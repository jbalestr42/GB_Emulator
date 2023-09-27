#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
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

    //mmu.loadRom(argv[1]);
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

        size_t ticks = cpu.update();
        timer.update(ticks);
        ppu.update(ticks);
        input.update();

		//std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}