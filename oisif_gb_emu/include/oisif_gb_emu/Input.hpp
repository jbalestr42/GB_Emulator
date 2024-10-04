#pragma once

#include <SFML/Graphics.hpp>

class MMU;

class Input
{
public:
	Input(MMU& mmu);
	~Input() = default;

	void tick();
	uint8_t read8();
	void write8(uint8_t v);
	std::uint8_t getValueFromInput(sf::Keyboard::Scan::Scancode scanCode);

	static const uint8_t InputCount = 8;

private:
	sf::Keyboard::Scan::Scancode _inputs[Input::InputCount];
	bool _inputsState[Input::InputCount];
	MMU& _mmu;
	uint8_t _joyReg;
};