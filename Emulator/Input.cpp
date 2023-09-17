#include "Input.hpp"
#include <iostream>

Input::Input()
{
	std::memset(_inputsState, 0, Input::InputCount);
	_inputs[0] = sf::Keyboard::Scan::Numpad0;
	_inputs[1] = sf::Keyboard::Scan::Numpad1;
	_inputs[2] = sf::Keyboard::Scan::Numpad2;
	_inputs[3] = sf::Keyboard::Scan::Numpad3;
	_inputs[4] = sf::Keyboard::Scan::Numpad4;
	_inputs[5] = sf::Keyboard::Scan::Numpad5;
	_inputs[6] = sf::Keyboard::Scan::Numpad6;
	_inputs[7] = sf::Keyboard::Scan::Numpad7;
	_inputs[8] = sf::Keyboard::Scan::Numpad8;
	_inputs[9] = sf::Keyboard::Scan::Numpad9;
	_inputs[10] = sf::Keyboard::Scan::A;
	_inputs[11] = sf::Keyboard::Scan::B;
	_inputs[12] = sf::Keyboard::Scan::C;
	_inputs[13] = sf::Keyboard::Scan::D;
	_inputs[14] = sf::Keyboard::Scan::E;
	_inputs[15] = sf::Keyboard::Scan::F;
}

std::uint8_t Input::getValueFromInput(sf::Keyboard::Scan::Scancode scanCode)
{
	for (std::uint8_t i = 0; i < Input::InputCount; i++)
	{
		if (scanCode == _inputs[i])
		{
			return i;
		}
	}
	return 0;
}

void Input::updateInputs()
{
	for (size_t i = 0; i < Input::InputCount; i++)
	{
		_inputsState[i] = sf::Keyboard::isKeyPressed(_inputs[i]);
	}
}