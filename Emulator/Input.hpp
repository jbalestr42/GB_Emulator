#pragma once

#include <SFML/Graphics.hpp>

class Input
{
public:
	Input();
	~Input() = default;

	std::uint8_t getValueFromInput(sf::Keyboard::Scan::Scancode scanCode);
	void updateInputs();
	bool isPressed(size_t p) { return _inputsState[p]; }

	static const uint8_t InputCount = 16;

private:
	sf::Keyboard::Scan::Scancode _inputs[Input::InputCount];
	bool _inputsState[Input::InputCount];
};