#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include "CPU.hpp"
#include "PPU.hpp"
#include "MMU.hpp"
#include "Timer.hpp"
#include "HardwareRegisters.hpp"

int main(int argc, char* argv[])
{
    MMU mmu;
    Interrupts interrupts(mmu);
    CPU cpu(mmu, interrupts);
    Timer timer(interrupts);
    PPU ppu(mmu, interrupts, 160, 144, 4, 60, "GAME BOY");

    // Hack until graphics is working
    //mmu.addMemoryOverride(HardwareRegisters::LY_ADDR, MMU::MemoryOverride([]() { return 0x90; }, nullptr));
    mmu.addMemoryOverride(HardwareRegisters::DIV_ADDR, MMU::MemoryOverride(
        [&timer]() { return static_cast<uint8_t>(timer.getDivRegister()); },
        [&timer](uint8_t value) { timer.resetDivRegister(); }));
    mmu.addMemoryOverride(HardwareRegisters::TIMA_ADDR, MMU::MemoryOverride(
        [&timer]() { return static_cast<uint8_t>(timer.getTimerCounter()); },
        [&timer](uint8_t value) { timer.setTimerCounter(value); }));
    mmu.addMemoryOverride(HardwareRegisters::TMA_ADDR, MMU::MemoryOverride(
        [&timer]() { return timer.getTimerModulo(); },
        [&timer](uint8_t value) { timer.setTimerModulo(value); }));
    mmu.addMemoryOverride(HardwareRegisters::TAC_ADDR, MMU::MemoryOverride(
        [&timer]() { return timer.getTimerControl(); },
        [&timer](uint8_t value) { timer.setTimerControl(value); }));

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

		//std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}