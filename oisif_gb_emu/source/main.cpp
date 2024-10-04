#include "Emulator.hpp"
#include "SFMLDisplay.hpp"

int main(int argc, char* argv[])
{
	SFMLDisplay display(160, 144, 4, 60, "Game Boy");
    Emulator emulator(display);

    emulator.loadRom(argv[1]);

    emulator.initialize();
    emulator.enableBootRom(true);

    emulator.update();

    return 0;
}