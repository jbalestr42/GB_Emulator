#include "Emulator.hpp"

int main(int argc, char* argv[])
{
    Emulator emulator;
    
    emulator.loadRom(argv[1]);

    emulator.initialize();
    emulator.enableBootRom(true);

    emulator.update();

    return 0;
}